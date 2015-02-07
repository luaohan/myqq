#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "x_log.h"
#include "fill_packet.h"


int 
fill_packet(char *buf, int buf_len, char *data, int data_len, short data_type)
{
    assert(buf != NULL);
    assert(buf_len > 0);    

    int valid_packet_len = sizeof(int) + sizeof(short) + data_len;
    if (valid_packet_len  > buf_len) {
        return -1;
    }

    int packet_len = htonl(valid_packet_len);   
    int packet_type = htons(data_type);

    char *p = buf;
    memcpy(p, (char *)&packet_len, sizeof(int));
    p += sizeof(int);
    memcpy(p, (char *)&packet_type, sizeof(short));
    p += sizeof(short);

    //allow data is NULL, for send header only
    if (data == NULL || data_len <= 0) {        
        return valid_packet_len;
    }

    memcpy(p, data, data_len);  

    return valid_packet_len;   
}

