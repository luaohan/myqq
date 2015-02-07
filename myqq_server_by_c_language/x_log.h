// log.h (2014-06-17)
// Yan Gaofeng (yangaofeng@360.cn)
#ifndef __X_LOG_H__
#define __X_LOG_H__

#define INFO  0
#define WARN  1
#define ERROR 2
#define FATAL 3

#define XLOGINFO(fmt, ...) \
    x_log(INFO, __FILE__, __LINE__, fmt, ## __VA_ARGS__)

#define XLOGWARN(fmt, ...) \
    x_log(WARN, __FILE__, __LINE__, fmt, ## __VA_ARGS__)

#define XLOGERROR(fmt, ...) \
    x_log(ERROR, __FILE__, __LINE__, fmt, ## __VA_ARGS__)

#define XLOGFATAL(fmt, ...) \
    x_log(FATAL, __FILE__, __LINE__, fmt, ## __VA_ARGS__)


int x_log_init(const char *log_file, int log_level, int log_file_size);

void x_log(int level, 
        const char *file_name, int line, 
        const char *fmt, ...);

void x_log_close();

//call after fork
int x_log_reinit();

#endif /*__X_LOG_H__*/

