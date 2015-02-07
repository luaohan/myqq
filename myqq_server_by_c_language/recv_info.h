// recv_file.h (2014-04-28)
// Yan Gaofeng (yangaofeng@360.cn)
#ifndef _recv_info_h_
#define _recv_info_h_

#define UserName_PassWd 1
#define PassWd_Ok 2
#define FriendList 3
#define ChatMess 4
#define FriendsStates 8

#define PassWd_Error -2
#define UserName_Error -1

#define NoHaveUser -5
#define NoFriends -3

typedef struct {
    char name[256];
    int fd;
}NAME_FD;

#define NUM 10

int recv_info(int client_fd, MYSQL *mysql, NAME_FD *nf);

#endif
