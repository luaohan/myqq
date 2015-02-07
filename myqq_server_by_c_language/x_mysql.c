#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "/usr/local/mysql/include/mysql/mysql.h"
#include "x_mysql.h"

int x_find_user(MYSQL *mysql, const char *username){

    assert(mysql!= NULL);
    assert(username != NULL);

    char sql[256];
    sprintf(sql, "select * from qquser where binary username = \'%s\'", username);
    int ret = mysql_real_query(mysql, sql, strlen(sql));
    if (ret != 0){
        fprintf(stderr, "mysql_real_query : %s \n", mysql_error(mysql));
        exit(0);
    }

    MYSQL_RES *res = mysql_store_result(mysql); 
    if (res == NULL){
        if (mysql_errno(mysql) == 0){
            printf("no have any results \n");
            return FALSE;
        } else {
            fprintf(stderr, "mysql_store_result : %s \n", mysql_error(mysql));
            exit(0);
        }
    }

    if (mysql_num_rows(res) == 1){
        mysql_free_result(res);
        return OK;
    } else {
        mysql_free_result(res);
        return FALSE;
    }

}


int 
x_check_pass_by_name( MYSQL *mysql, const char *username, const char *password){

    char sql[256];
    sprintf(sql, "select passwd from qquser where binary username = '%s'", username);
    int ret = mysql_real_query(mysql, sql, strlen(sql));
    if (ret != 0){
        fprintf(stderr, "mysql_real_query : %s \n", mysql_error(mysql));
        exit(0);
    }

    MYSQL_RES *res = mysql_store_result(mysql);
    if (res == NULL){
        if (mysql_errno(mysql) == 0){ 
            printf("no have any password \n");
            return FALSE;
        } else {
            fprintf(stderr, "mysql_store_result : %s \n", mysql_error(mysql));
            exit(0);
        }   
    }   

    MYSQL_ROW row = mysql_fetch_row(res);
    char *passwd = row[0];

    if (strcmp(password, passwd) == 0){
        mysql_free_result(res);
        return OK;
    }

    mysql_free_result(res);
    return FALSE;
}

int 
x_find_friendname_by_username(MYSQL *mysql, const char *username, char *friendname){
    char sql[256];
    sprintf(sql, "select friendname from qqfriends where binary username = '%s'", username);
    int ret = mysql_real_query(mysql, sql, strlen(sql));
    if (ret != 0){
        fprintf(stderr, "mysql_real_query : %s \n", mysql_error(mysql));
        exit(0);
    }

    MYSQL_RES *res = mysql_store_result(mysql);
    if (res == NULL){
        if (mysql_errno(mysql) == 0){ 
            printf("no have any friends \n");
            return FALSE;
        } else {
            fprintf(stderr, "mysql_store_result : %s \n", mysql_error(mysql));
            exit(0);
        }   
    } 

    if (mysql_num_rows(res) == 0){//no have friends
        mysql_free_result(res);
        return FALSE;
    }

    memset(friendname, 0, 1024);
    MYSQL_ROW row;
    while( (row = mysql_fetch_row(res)) != NULL){
        strcat(friendname, row[0]);
        strcat(friendname, " ");  
    }   

    mysql_free_result(res);
    return OK;
}

void
x_update_state_by_username(MYSQL *mysql, const char *username, int state){
    char sql[256];
    sprintf(sql, "update qquser set state = %d where username = '%s'", state, username);
    int ret = mysql_real_query(mysql, sql, strlen(sql));
    if (ret != 0){
        fprintf(stderr, "mysql_real_query : %s \n", mysql_error(mysql));
        exit(0);
    }
    return ;
}

int 
x_find_friendstate_by_username(MYSQL *mysql, const char *username, char *states){
    char friendnames[1024];
    int ret = x_find_friendname_by_username(mysql, username, friendnames);
    if (ret == -1){ //have no friends
        return FALSE; 
    }

    char *seps = " ";
    char *token = strtok(friendnames, seps);
    MYSQL_RES *res;
    MYSQL_ROW row;

    memset(states, 0, 1024);
    while (token){
        char sql[256];
        sprintf(sql, "select state from qquser where binary username = '%s'", token);
        ret = mysql_real_query(mysql, sql, strlen(sql));
        if (ret != 0){
            fprintf(stderr, "mysql_real_query : %s \n", mysql_error(mysql));
            exit(0);
        }

        res = mysql_store_result(mysql);
        if (res == NULL){
            if (mysql_errno(mysql) == 0){ 
                printf("no have any friends \n");
                return FALSE;
            } else {
                fprintf(stderr, "mysql_store_result : %s \n", mysql_error(mysql));
                exit(0);
            }   
        }

        while( (row = mysql_fetch_row(res)) != NULL){
            strcat(states, row[0]);
            strcat(states, " ");  
        }   

        token = strtok(NULL, seps);
    }

    mysql_free_result(res);
    return OK;
}

