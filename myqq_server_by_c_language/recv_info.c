#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

#include "/usr/local/mysql/include/mysql/mysql.h" 
#include "recv_info.h"
#include "fill_packet.h"
#include "x_log.h"
#include "x_mysql.h"
#include "x_socket.h"
#include "worker.h"

#define BUFFER_SIZE 8192
#define Packet_Head 4
#define Packet_Type 2 
#define Packet_H_T  6

int OnChatMess(MYSQL *mysql, int cli_fd, char *buf, NAME_FD *nf );
int OnUserName_PassWd(MYSQL *mysql,int cli_fd, char *buf, NAME_FD *nf );
int user_tell_friend_state(char *username, int state, NAME_FD *nf, MYSQL *mysql);
int do_if_cli_exit(int cli_fd, NAME_FD *nf, MYSQL *mysql);
int get_friends_states(int cli_fd, MYSQL *mysql, NAME_FD *nf);

int recv_info(int client_fd, MYSQL *mysql, NAME_FD *nf){

    assert (mysql!= NULL);
    assert (nf != NULL);

    int i;
    char buf[BUFFER_SIZE];
    int read_size = x_recv(client_fd, buf, Packet_Head);
    if (read_size == 0) {
        int ret = do_if_cli_exit(client_fd, nf, mysql);
        if (ret < 0){
            return -1;
        }
        return CLOSE_CLI_FD; //client close
    } else if (read_size < 0){
        if (errno == 104){ // connection reset
            return ;
        }
        XLOGERROR("x_recv err : %d :%s \n", errno, strerror(errno));
        return -1;
    }

    int packet_len = htonl( *((int *)buf) );

    read_size = x_recv(client_fd, buf, Packet_Type);
#if 0        
    if (read_size == 0){ //client close
        int ret = do_if_cli_exit(client_fd, nf, mysql);
        if (ret < 0){
            return -1;
        }
        return -2;
    } else 
#endif
        if (read_size < 0){
            XLOGERROR("x_recv err \n");
            return -1;
        }

    int packet_type = htons( *((short *)buf) );
    int body_len = packet_len - Packet_H_T;

    //read packet body
    read_size = x_recv(client_fd, buf, body_len);
#if 0
    if (read_size == 0){
        int ret = do_if_cli_exit(client_fd, nf, mysql);
        if (ret < 0){
            return -1;
        }
        return -2;
    } else
#endif
        if ( read_size < 0) {
            XLOGERROR("recv packet body error, %s", strerror(errno));
            return -1; 
        }

    buf[body_len] = '\0';
    printf("body : %s \n", buf);
    int ret = -1;
    switch (packet_type) {
        case UserName_PassWd:
            ret = OnUserName_PassWd(mysql, client_fd, buf, nf);
            if (ret == -1){
                return -1;
            }
            if (ret == NoHaveUser || ret == PassWd_Error){
                return CLOSE_CLI_FD; //close cli_fd
            }

            if (ret == NoFriends){ //no have friend
                return 0;
            }

            //get friends states
            ret = get_friends_states(client_fd, mysql, nf);
            //no have to check it's ret 

            return 0;
            break;
        case ChatMess:
            return OnChatMess(mysql, client_fd, buf, nf);
            break;
        default:
            XLOGINFO("invalid packet type: %d\n", packet_type);
            return -1;
            break;
    }
    return 0;
}

int OnUserName_PassWd(MYSQL *mysql,int client_fd, char *buf, NAME_FD *nf)
{
    assert(mysql != NULL);

    int i = 0;
    char *username_passwd = buf;
    char *username = NULL;
    char *passwd = NULL;

    char *seps = " ";
    char *token = strtok(username_passwd, seps); //a token is username, passwd 
    if (token != NULL){
        username = token;
    }

    token = strtok(NULL, seps);
    if (token != NULL){
        passwd = token;
    }

    char response[BUFFER_SIZE];
    int ret = x_find_user(mysql, username);
    if (ret == 0){ //have the user
        //check his passwd
        ret = x_check_pass_by_name(mysql, username, passwd);
        if (ret == 0){  //pass is ok
            //在数组中登记
            for (i = 0; i < NUM; i++){
                if (nf[i].fd <= 0){
                    nf[i].fd = client_fd;
                    strcpy(nf[i].name, username);
                    break;
                }
            }

            x_update_state_by_username(mysql, username, ON); 
            //tell his friends he is online
            ret = user_tell_friend_state(username, ON, nf, mysql);
            if (ret == -1){
                return -1;   //
            } else if (ret == NoFriends){ //no have friends
                //let he login
                int len = fill_packet(response, BUFFER_SIZE, NULL, 0, PassWd_Ok);
                if (len <= 0) {
                    return -1;
                }

                int write_size = write(client_fd, response, len);
                if (write_size != len) {
                    XLOGERROR("recv_file write LoginOk err: %s", strerror(errno));
                    return -1;
                }

                return NoFriends;
            }
            //have friends
            char friendnames[BUFFER_SIZE];
            ret = x_find_friendname_by_username(mysql, username, friendnames);
            //do not have to check (ret < 0)
            int len = fill_packet(response, BUFFER_SIZE, 
                    friendnames, strlen(friendnames), FriendList);
            if (len <= 0) {
                return -1;
            }

            int write_size = write(client_fd, response, len);
            if (write_size != len) {
                XLOGERROR("recv_file write LoginOk err: %s", strerror(errno));
                return -1;
            }

            return 0;
        } else { //pass is not ok
            int len = fill_packet(response, BUFFER_SIZE, NULL, 0, PassWd_Error);
            if (len <= 0) {
                return -1;
            }

            int write_size = write(client_fd, response, len);
            if (write_size != len) {
                XLOGERROR("recv_file write LoginOk err: %s", strerror(errno));
                return -1;
            }
            return PassWd_Error;  //
        }
    } else { //no have the user

        XLOGINFO("invalid user name:%s \n", buf); 

        int len = fill_packet(response, BUFFER_SIZE, NULL, 0, UserName_Error);
        int write_size = write(client_fd, response, len);
        if (write_size != len) {
            XLOGERROR("recv_file write LoginOk err: %s", strerror(errno));
            return -1;
        }
        return NoHaveUser;
    }
    return 0;
}

int OnChatMess(MYSQL *mysql,int client_fd, char *buf, NAME_FD *nf)
{
    char res[BUFFER_SIZE];
    char friend_name_len[5] = {0};
    memcpy(friend_name_len, buf, 4);

    int name_len = atoi(friend_name_len);

    char *pname = buf + 4;
    char friend_name[256] = {0};
    memcpy(friend_name, pname, name_len);
    char *chat_info = pname + name_len;

    //find now username
    char username[128];
    int i = 0;
    for (i = 0; i < NUM; i++){
        if (nf[i].fd == client_fd){
            memcpy(username, nf[i].name, strlen(nf[i].name));
            break;
        }
    }

    username[strlen(username)] = '\0';
    strcat(username, " : \r\n   ");

    char username_info[BUFFER_SIZE];
    memcpy(username_info, username, strlen(username));

    memset(username, 0, 128);
    strcat(username_info, chat_info);

    for (i = 0; i < NUM; i++){
        if (strcmp(nf[i].name, friend_name) == 0) {
            int len = 
                fill_packet(res, BUFFER_SIZE, username_info, strlen(username_info), ChatMess);

            int write_size = write(nf[i].fd, res, len);
            if (write_size < 0){
                XLOGERROR("write to friend err \n");
                return -1;
            }
            memset(username_info, 0, BUFFER_SIZE);
            break;
        }
    }

    return 0;
}


int
user_tell_friend_state(char *username, int state, NAME_FD *nf, MYSQL *mysql){

    //find his all friends
    char friendsnames[BUFFER_SIZE];
    int ret = x_find_friendname_by_username(mysql, username, friendsnames);
    if (ret == -1){  //have no friends
        return NoFriends;
    }

    char *seps = " ";
    char *token = strtok(friendsnames, seps); //a token is friend
    while (token){
        //printf("%s \n", token);
        int j = 0;
        for (j = 0; j < NUM; j++){
            char *friendname = nf[j].name;
            char friend_fd = nf[j].fd;
            if (strcmp(token, friendname) == 0){
                char res[256];
                int len = fill_packet(res, BUFFER_SIZE, username, strlen(username), state);

                int write_size = write(friend_fd, res, len);
                if (write_size != len){
                    XLOGERROR("recv_file write LoginOk err: %s", strerror(errno));
                    return -1;
                }
                break;
            }
        }
        token = strtok(NULL, seps);
    }      

    return 0;
}

int do_if_cli_exit(int client_fd, NAME_FD *nf, MYSQL *mysql){

    int i = 0;
    for (i = 0; i < NUM; i++){
        if (nf[i].fd == client_fd){
            nf[i].fd = -1;
            char *username = nf[i].name;
            //update his state
            x_update_state_by_username(mysql, username, OFF);

            //tell his friends he is OFF
            int ret = user_tell_friend_state( username, OFF, nf, mysql);
            if (ret < 0){
                return -1;
            }

            memset(nf[i].name, 0, 256);
            break;
        }
    }
    return 0;
}

int get_friends_states( int client_fd, MYSQL *mysql, NAME_FD *nf){

    int i = 0;
    const char *username;
    for (i = 0; i < NUM; i++){
        if (nf[i].fd == client_fd){
            username = nf[i].name;
        }
    }

    char friends_states[BUFFER_SIZE];
    int ret = x_find_friendstate_by_username(mysql, username, friends_states);
    if (ret != 0){ //no have friends
        return -1;
    }

    char res[BUFFER_SIZE];
    int len = fill_packet(res, BUFFER_SIZE, friends_states, 
            strlen(friends_states), FriendsStates);
    if (len <= 0) {
        return -1;
    }

    int write_size = write(client_fd, res, len);
    if (write_size != len){
        XLOGERROR("recv_file write LoginOk err: %s", strerror(errno));
        return -1;
    }

    return 0;
}
