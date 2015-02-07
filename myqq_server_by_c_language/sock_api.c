#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "sock_api.h"
#include "x_log.h"

int set_no_block(int fd){
    int flags;
    flags = fcntl(fd, F_GETFL, NULL);
    if (flags < 0){
        return -1;
    }

    int ret;
    ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if (ret < 0){
        return -1;
    }

    return 0;
}

int set_sock_opt( int fd ){
    int on = 1;
    int ret;
    ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
    if (ret < 0){
        return -1;
    }

    on = 1;
    ret = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(on));
    if (ret < 0){
        return -1;
    }

    return 0;

}

/* disable nagle on socket */                           
void x_set_sock_no_delay(int s)
{
    int opt;
    socklen_t optlen;

    optlen = sizeof opt;
    if (getsockopt(s, IPPROTO_TCP, 1/*TCP_NODELAY*/, &opt, &optlen) == -1) {
        return;
    }   

    if (opt == 1) {
        return;
    }   
    opt = 1;
    setsockopt(s, IPPROTO_TCP, 1/*TCP_NODELAY*/, &opt, sizeof( opt )); 

    return;
}

int create_server( int port ){

    int ser_fd;
    ser_fd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( ser_fd < 0 ){
        //perror("socket error");
        XLOGERROR("socket error");
        return -1;
    }

    x_set_sock_no_delay(ser_fd);

    int ret;
    ret = set_sock_opt(ser_fd);
    if (ret < 0){
        //perror("set_sock_opt error");
        XLOGERROR("set_sock_opt error");
        return -1;
    }

    struct sockaddr_in ser_addr;
    bzero( &ser_addr, sizeof(ser_addr) );
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons( port );
    ser_addr.sin_addr.s_addr = htonl( INADDR_ANY );

    ret = bind( ser_fd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    if ( ret < 0 ){
        //perror("bind error");
        XLOGERROR("bind error");
        return -1;
    }

    ret = listen( ser_fd, 5 );
    if (ret < 0){
        XLOGERROR("listen error");
        //perror("listen error");
        return -1;
    }

    return ser_fd;
}


