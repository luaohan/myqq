#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/timeb.h>
#include <stdarg.h>
#include <assert.h>

#include "x_log.h"

#define MAX_BUFFER_SIZE 256 * 1024

//set by external
static int x_log_level = INFO;    ///////
static char x_log_path[PATH_MAX];  
static char x_log_file[64];   /////////
static size_t x_log_file_size = 0;   ///////

//used by internal
static int x_log_fd = -1;     /////////
static size_t x_log_file_index = 0;
static size_t x_log_cur_file_size = 0;

static void rollover()
{
    char src_file[PATH_MAX];
    char dst_file[PATH_MAX];
    int i;
    int len;
    for (i = x_log_file_index; i > 0; i--) {
        len = snprintf(src_file, PATH_MAX, "%s/%s.%d", x_log_path, x_log_file, i);
        if (len == -1) {
            return;
        }

        len = snprintf(dst_file, PATH_MAX, "%s/%s.%d", x_log_path, x_log_file, i + 1);
        if (len == -1) {
            return;
        }

        int rc = rename(src_file, dst_file);
        if (rc == -1) {
            XLOGWARN("rollover file %s to %s error: %s\n", 
                    src_file, 
                    dst_file,
                    strerror(errno)
                    );
            return;
        }
    }

    len = snprintf(src_file, PATH_MAX, "%s/%s", x_log_path, x_log_file);
    if (len == -1) {
        return;
    }

    len = snprintf(dst_file, PATH_MAX, "%s/%s.1", x_log_path, x_log_file);
    if (len == -1) {
        return;
    }

    close(x_log_fd);
    x_log_fd = -1;

    int rc = rename(src_file, dst_file);
    if (rc == -1) {
        XLOGWARN("rollover file %s to %s error: %s\n", 
                src_file, 
                dst_file,
                strerror(errno)
                );
        return;
    }

    x_log_fd = open(src_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (x_log_fd == -1) {
        return;
    }

    x_log_file_index++;
}

//If use daemon process, Ensure that the log_file is an absolute path
//return 0 on success
int x_log_init(const char *log_file, int log_level, int log_file_size)
{
    assert(log_file != NULL);
    assert(log_level >= INFO && log_level <= FATAL);

    if (x_log_fd != -1) {
        return -1;
    }

    char *log_file_1 = strdup(log_file);
    if (log_file_1 == NULL) {
        return -1;
    }

    char *path = dirname(log_file_1);
    if (path == NULL) {
        free(log_file_1);
        return -1;
    }

    int len = snprintf(x_log_path, PATH_MAX, "%s", path);
    if (len == -1) {
        free(log_file_1);
        return -1;
    }

    free(log_file_1);


    char *log_file_2 = strdup(log_file);
    if (log_file_2 == NULL) {
        return -1;
    }

    char *file = basename(log_file_2);
    if (file == NULL) {
        free(log_file_2);
        return -1;
    }

    len = snprintf(x_log_file, 64, "%s", file);
    if (len == -1) {
        free(log_file_2);
        return -1;
    }

    free(log_file_2);

    x_log_fd = open(log_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (x_log_fd == -1) {
        return -1;
    }

    x_log_level = log_level;

    x_log_file_size = log_file_size;
    if (x_log_file_size < 1024) {
        x_log_file_size = 1024;
    }

    return 0;
}

//只是重新初始化一次fd
int x_log_reinit()
{
    close(x_log_fd);
    char log_file[PATH_MAX];
    int len = snprintf(log_file, PATH_MAX, "%s/%s", x_log_path, x_log_file);
    if (len == -1) {
        return -1;
    }

    x_log_fd = open(log_file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (x_log_fd == -1) {
        return -1;
    }

    return 0;
}

//return write bytes
void x_log(int level, 
        const char *file_name, int line, 
        const char *fmt, ...)
{
    assert(file_name != NULL);
    assert(fmt != NULL);

    if (x_log_fd == -1) {
        return;
    }

    if (level < x_log_level) {
        return;
    }

    //note the buffer size limit
    char buffer[MAX_BUFFER_SIZE];    //256K
    char *p = buffer;
    int remain = MAX_BUFFER_SIZE;

    time_t  timenow;
    struct  timeb  tmb;
    struct  tm     tmstru;

    char *str_level;
    switch (level) {
        case INFO:
            str_level = "INFO";
            break;
        case WARN:
            str_level = "WARN";
            break;
        case ERROR:
            str_level = "ERROR";
            break;
        case FATAL:
            str_level = "FATAL";
            break;
        default:
            break;
    }

    time(&timenow);
    localtime_r(&timenow, &tmstru);
    ftime(&tmb);

    int len = sprintf(p, 
            "%4d-%02d-%02d %02d:%02d:%02d:%03d\t[%s]\t<%s:%d>\t",
            (tmstru.tm_year + 1900),
            (tmstru.tm_mon + 1),
            tmstru.tm_mday,
            tmstru.tm_hour,
            tmstru.tm_min,
            tmstru.tm_sec,
            tmb.millitm,
            str_level,
            file_name,
            line);

    if (len >= MAX_BUFFER_SIZE) {
        return;
    }

    p += len;
    remain -= len;

    /* Guess we need no more than 1024 bytes. */
    int n;
    va_list ap;

    va_start(ap, fmt);
    n = vsnprintf(p, remain, fmt, ap);
    va_end(ap);

    if (n <= -1 || n > remain) {
        //no enough buffer
        return;
    }

    p += n;
    len += n;
    remain -= n;

    //should process EINTR signal
    int write_bytes = write(x_log_fd, buffer, len);
    if (write_bytes != len) {
        //error process
        return;
    }

    x_log_cur_file_size += len;

    if (x_log_cur_file_size >= x_log_file_size) {
        rollover();
        x_log_cur_file_size = 0;
    }
}

void x_log_close()
{
    if (x_log_fd == -1) {
        return;
    }

    close(x_log_fd);

    x_log_fd = -1;
    x_log_level = 0;
}

