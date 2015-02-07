// x_mysql.h (2014-08-18)
// WangPeng (1245268612@qq.com)

#ifndef _X_MYSQL_H_
#define _X_MYSQL_H_

#define LOGINOK 0    
#define USERNAME 1
#define PASSWORD 2
#define FRIENDLIST 3
#define CHATMESS 4
#define LOGINERR 5
#define FRIENDNUMS 6 
#define CHATTOFRIEND 7

#define ON 1
#define OFF 0

#define OK 0
#define FALSE -1

int 
x_find_user(MYSQL *mysql, const char *username);

int 
x_check_pass_by_name(MYSQL *mysql, const char *username, const char *password);

int 
x_find_friendname_by_username(MYSQL *mysql, const char *username, char *friendname);

void 
x_update_state_by_username(MYSQL *mysql, const char *username, int state);

int 
x_find_friendstate_by_username(MYSQL *mysql, const char *username, char *states);


#endif
