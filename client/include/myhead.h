#ifndef __MYHEAD__H
#define __MYHEAD__H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sqlite3.h>
#include <pthread.h>
#include <time.h>
#include <termios.h>

#define MAX_SIZE 1024
#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)

enum action{REG, LOGIN, LIST, CHAT, STOALL, OFFLINE, CHANGEPWD, CHANGENAME, VIEWMSG, OFFLINEMSG, OUT,BAN,RMBAN,NAME,ID};
enum result{SUCCESS = 1, FAILURE, PWERR, NOUSR, EMPTY,NOTONLINE,ONLINE,SINGAL,ALL};

typedef struct message
{
    int action;
    int flag;
    char name[MAX_SIZE];
    long id;
    char password[MAX_SIZE];
    char toname[MAX_SIZE];
    char msg[MAX_SIZE];
}Msg;

typedef struct online
{
    int cfd;
    int flag;
    char name[MAX_SIZE];
}Online;

//聊天界面
extern void show_chat();
//登陆界面
extern void show_login();
//用户注册
extern int reg(int sfd, Msg *msg);
//用户登陆
extern int login(int sfd, Msg *msg, char *str);
//界面跳转
extern void show_msg(char *str);
//线程
extern void *pthread_read(void *arg);
//显示在线
extern int list(Msg *msg);

#endif
