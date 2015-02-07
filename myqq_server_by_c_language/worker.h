// worker.h (2014-07-25)
// WangPeng (1245268612@qq.com)

#ifndef _WORKER_SELECT_H
#define _WORKER_SELECT_H

#define CLOSE_CLI_FD -2

void *worker_select(void *arg);
static int select_client_num = 0;

#endif
