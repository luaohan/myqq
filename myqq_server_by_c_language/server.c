#include <stdio.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "sock_api.h"
#include "worker.h"
#include "x_log.h"

int SER_PORT = 8899;
int THREAD_NUM = 1;

int ser_fd = -1;
int flags = 1;

void sig_pro(int sig){
    flags = 0;
    close(ser_fd);
    ser_fd = -1;
    return ;
}

void usage(char **argv){
    printf("Usage : <%s> [-p|-n]\n", argv[0]);
    printf("after 'p' is the port of server\n");
    printf("after 'n' is the numbers of thread\n");
    return ;
}


int main(int argc, char **argv){

    x_log_init("./ERROR.txt", INFO, 512*1024*1024);

    int opt;
    char *optstring = "p:n:";
    while ((opt = getopt(argc, argv, optstring)) != -1){
        if (opt == 'p'){
            int p = atoi(optarg);
            if ( ( p < 5000) || (p > 40000)){
                XLOGINFO("the port of server should in 5000~40000\n");
                return -1;
            }
            SER_PORT = p;
        } else if (opt == 'n'){
            int n = atoi(optarg);
            if( !( n >= 0)){
                XLOGINFO("the number of thread should beyond 0 or be 0\n");
                return -1;
            }
            THREAD_NUM = n;
        } else {
            usage(argv);
            return -1;
        }
    }

    if (optind < argc){
        usage(argv);
        return -1;
    }

    struct sigaction act;
    act.sa_handler = sig_pro;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT, &act, NULL);
    sigaction(SIGTERM, &act, NULL);
    sigaction(SIGPIPE, NULL, NULL);


    ser_fd = create_server(SER_PORT);

    int pipefd[THREAD_NUM][2];
    int i = 0;
    int ret;
    pthread_t tid[THREAD_NUM];

    for (i = 0; i < THREAD_NUM; i++){
        ret = pipe(pipefd[i]);
        if (ret < 0){
            XLOGERROR("pipe error\n");
            return -1;
        }

        ret = pthread_create(&tid[i], NULL, worker_select, (void *)pipefd[i][0]);
        if (ret != 0){
            XLOGERROR("pthread_create error\n");
            return -1;
        }
    }

    int cli_fd;
    int counter = 0;

    while (flags == 1) {

        struct sockaddr_in cli_addr;
        socklen_t len = sizeof(ser_fd);
        cli_fd = accept( ser_fd, (struct sockaddr *)&cli_addr, &len);
        if (cli_fd < 0) {
            if (errno == EINTR){
                XLOGINFO("recv EINTR signal \n");
                continue;
            }

            XLOGERROR("accept error");
            return -1;
        }

        if (counter >= THREAD_NUM) {
            counter = 0;
        }

        int write_size;
again:
        write_size = write(pipefd[counter][1], &cli_fd, sizeof(cli_fd));
        counter++;
        if (write_size != sizeof(cli_fd)) {
            if (errno == EINTR) {
                XLOGINFO("recv EINTR signal");
                goto again;
            }

            return -1;
        }

        XLOGINFO("write to pipe ok\n");
    }

    for (i = 0; i < THREAD_NUM; i++){
        XLOGINFO("pthread join, tid: %llu \n", tid[i]);
        pthread_join(tid[i], NULL);
    }

    XLOGINFO("server sucess exit.\n");

    return 0;
}
