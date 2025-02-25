#ifndef _MYHEAD_H_
#define _MYHEAD_H_

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

#define MAX_SIZE 1024

enum action{REG, LOGIN, LIST, CHAT,STOALL, OFFLINE, CHANGEPWD, CHANGENAME, VIEWMSG, OFFLINEMSG, OUT, BAN, RMBAN,NAME,ID}; //所实现的功能
enum result{SUCCESS = 1, FAILURE, PWERR, NOUSR, EMPTY, NOTLINE, ONLINE,SINGAL,ALL}; //各个功能函数返回值

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
    struct online *next;
}Online;

typedef struct online * Link;
//tcp服务器初始化函数
extern int tcp_server_init(const char *ip, int port, int backlog);
//服务器等待客户端连接函数
extern int tcp_server_wait(int fd);
extern int ban(int cfd, Msg *msg);
extern int chat(int cfd, Msg *msg);
extern int insertmsg(Msg *msg); 
//给每一个连接的客户端开辟一个线程
extern void *pthread_read(void *arg);
//注册
extern int reg(int cfd, Msg *msg);
//登陆
extern int login(int cfd, Msg *msg);
//查看在线人数
extern int online(int cfd, Msg *msg);
extern int list(int cfd, Msg *msg);
extern int viewmsg(int cfd, Msg *msg);
extern int viewoffmsg(int cfd, Msg *msg);
extern int changename(int cfd, Msg *msg);
extern int changepwd(int cfd, Msg *msg);
extern int insertmsgall(Msg *msg);
extern int insertoffmsg(int cfd, Msg *msg);
extern int offline(int cfd, Msg *msg);
//管理员解除禁言
extern int rmban(int cfd, Msg *msg);
extern int stoall(int cfd, Msg *msg);
//管理员踢人
extern int out(int cfd,Msg *msg);

#endif
