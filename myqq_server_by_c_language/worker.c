#include <pthread.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "/usr/local/mysql/include/mysql/mysql.h"
#include <assert.h>

#include "sock_api.h"
#include "x_log.h"
#include "worker.h"
#include "fill_packet.h"
#include "recv_info.h"

extern int flags;
//extern int select_client_num;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define SUB(select_client_num)  {  \
            pthread_mutex_lock(&mutex); \
            select_client_num--; \
            pthread_mutex_unlock(&mutex); \
}

#define ADD(select_client_num)  {  \
            pthread_mutex_lock(&mutex); \
            select_client_num++; \
            pthread_mutex_unlock(&mutex); \
}

void *worker_select(void *arg){
    
    MYSQL *mysql = mysql_init(NULL);
    if ( mysql == NULL){
        fprintf(stderr, "mysql_init : %s \n", mysql_error(mysql));
        exit(0);
    }

    MYSQL *p = mysql_real_connect(
            mysql, "localhost", "root", "root", "qq", 3306, NULL, 0);
    if (p == NULL){ 
        fprintf(stderr, "mysql_real_connect : %s \n", mysql_error(mysql)); 
        exit(0);
    }

    NAME_FD *nf = (NAME_FD *)malloc( NUM * sizeof(NAME_FD) );
    if (nf == NULL){
        printf("malloc error \n");
        exit(0);
    }

    memset(nf, 0, NUM * sizeof(NAME_FD));

    int pipefd = (int)arg;

    int client_fd[FD_SETSIZE];

    int i = 0;
    for (i = 0; i < FD_SETSIZE; i++){
        client_fd[i] = -1;
    }

    fd_set rset;
    struct timeval tv;
    int maxfd = pipefd;

    while (flags == 1){

        tv.tv_sec = 3;
        tv.tv_usec = 0;

        FD_ZERO(&rset);
        for (i = 0; i < maxfd; i++){
            if (client_fd[i] > 0){
                FD_SET(client_fd[i], &rset);
                XLOGINFO("now set client : %d\n", client_fd[i]);
                if (maxfd < client_fd[i]){
                    maxfd = client_fd[i];
                }
            }
        }

        FD_SET(pipefd, &rset);

        int nready;
        nready = select(maxfd + 1, &rset, NULL, NULL, &tv);
        if (nready == 0){
            XLOGINFO("now the number of clients is : %d\n", select_client_num);
            continue;
        } else if (nready < 0){
            if (errno == EINTR){
                continue;
            }
            XLOGERROR("select error\n");
            return ;
        }

        if (FD_ISSET(pipefd, &rset)){

            int cli_fd;
            int read_size;
again:
            read_size = read(pipefd, &cli_fd, sizeof(cli_fd));
            if (read_size != sizeof(cli_fd)) {
                if (errno == EINTR){
                    goto again;
                }

                XLOGERROR("read error\n");
                return ;
            }

            ADD(select_client_num);

            for (i = 0; i < FD_SETSIZE; i++){
                if (client_fd[i] < 0){
                    client_fd[i] = cli_fd;
                    break;
                }
            }

        }

        for (i = 0; i < maxfd; i++){
            if (client_fd[i] < 0){
                continue;
            }

            if (FD_ISSET(client_fd[i], &rset)){

                int ret = recv_info(client_fd[i], mysql, nf);
                if (ret == CLOSE_CLI_FD){
                    SUB(select_client_num);
                    XLOGINFO("close fd: %d \n", client_fd[i]);
                    close(client_fd[i]);
                    //shutdown(client_fd[i], SHUT_RDWR);
                    client_fd[i] = -1;
                    continue;
                } else if(ret == -1){
                    XLOGERROR("read error \n"); //
                    //return ; 
                }
                
                XLOGINFO("from recv_file() return \n");
            }

        }


    }

    XLOGINFO("thread %u success exit\n", pthread_self());
    pthread_exit(NULL);
    return ;

}
