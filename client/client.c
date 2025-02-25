#include "./include/myhead.h"

void show_msg(char *str)
{
    int i = 3;
    while(i)
    {
        system("clear");
        printf("%s,界面将在%d秒后跳转!\n",str,i--);
        sleep(1);
    }
}

int main(int argc, char *argv[])
{ 
    int flag = 0;
	char menu[MAX_SIZE];
    char str[MAX_SIZE];
	char temp[MAX_SIZE];
    memset(str,0,sizeof(str));
    pthread_t id;
    Msg *msg = (Msg *)malloc(sizeof(Msg));
    
    if(argc < 3){
        printf("Usage:%s <IP> <PORT>\n", argv[0]);
        return -1;
    }

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sfd){
        perror("socket");
        return -1;
    }

    //构建存储服务器地址信息的结构体变量
    struct sockaddr_in saddr;
    saddr.sin_family      = AF_INET;
    saddr.sin_port        = htons( atoi(argv[2]) );
    saddr.sin_addr.s_addr = inet_addr(argv[1]);

    //向服务器发起连接请求
    if(-1 == connect(sfd, (void *)&saddr, sizeof saddr)){
        perror("connect");
        return -1;
    }
    show_login();
    while(1)
    {
        printf("\t\t请输入您要选择的功能(reg/login/exit):");
		scanf("%s",menu);

		if(strcmp(menu,"reg") == 0)
	    {
	        reg(sfd, msg);
	    }
	    else if(strcmp(menu,"login") == 0)
	    {
			if(login(sfd, msg, str) == SUCCESS)
			{
				flag = 1;
			}
			else
			{
			    flag = 0;
			}
	   	}
	    else if(strcmp(menu,"exit") == 0)
	    {
	        exit(1);
	    }
		else
		{
	        printf("\t\t输入的格式不正确!\n\n");
		}

		if(flag == 1)
		{
		    break;
		}
	}

	if(strcmp("admin",str) == 0)
	{
	    show_chat();
		printf("\t\t|您是超级用户,可以使用以下功能:                       |\n");
		printf("\t\t|                                                     |\n");
		printf("\t\t|                    *踢出某人(out)                   |\n");
		printf("\t\t|                    *禁言某人(ban)                   |\n");
		printf("\t\t|                    *解除某人禁言(rmban)             |\n");
		printf("\t\t|                                                     |\n");
		printf("\t\t+=====================================================+\n");
	}
	else
	{
	    show_chat();
	}
	printf("\t\t当前用户为:%s\n",str);
	pthread_create(&id, NULL, pthread_read, (void *)&sfd);

    printf("\t\t请输入您要选择的功能:\n");
	while(1)
	{
		scanf("%s",menu);

	    if(strcmp(menu,"online") == 0)
        {
	        bzero(msg, sizeof(Msg));
		    msg->action = LIST;
		    write(sfd, msg, sizeof(Msg));
		}
	    else if(strcmp(menu,"chatone") == 0)
			{
		    	bzero(msg, sizeof(Msg));
		    	msg->action = CHAT;
		    	strcpy(msg->name, str);
		    	printf("请输入联系人:");
		    	scanf("%s",msg->toname);
		    	printf("请输入需要发的消息:");
		    	scanf("%s",msg->msg);

		    	write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"chatall") == 0)
			{
		    	bzero(msg, sizeof(Msg));
		    	msg->action = STOALL;
		    	strcpy(msg->name, str);
				strcpy(msg->toname, "all");
		    	printf("请输入需要群发的消息:");
		    	scanf("%s",msg->msg);

		    	write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"quit") == 0)
			{
			    bzero(msg, sizeof(Msg));
				msg->action = OFFLINE;
				strcpy(msg->name, str);

				write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"changepwd") == 0)
			{
			    bzero(msg, sizeof(Msg));
				msg->action = CHANGEPWD;
				printf("请输入新密码:\n");
				scanf("%s",msg->password);
				strcpy(msg->name, str);

				write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"changename") == 0)
			{
			    bzero(msg, sizeof(Msg));
				msg->action = CHANGENAME;
				printf("请输入新昵称:\n");
				scanf("%s",temp);
				strcpy(msg->name, str);
				strcpy(msg->toname, temp);

				write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"help") == 0)
			{
				if(strcmp("admin",str) == 0)
				{
	                show_chat();
		            printf("\t\t|您是超级用户,可以使用以下功能:                       |\n");
		            printf("\t\t|                                                     |\n");
		            printf("\t\t|                    *踢出某人(out)                   |\n");
		            printf("\t\t|                    *禁言某人(ban)                   |\n");
		            printf("\t\t|                    *解除某人禁言(rmban)             |\n");
		            printf("\t\t|                                                     |\n");
		            printf("\t\t+=====================================================+\n");
				}
				else
				{
	    			show_chat();
				}
				printf("\t\t当前用户为:%s\n",str);
			}
	    else if(strcmp(menu,"viewmsg") == 0)
			{
			    printf("***查看聊天记录***\n");
				bzero(msg, sizeof(Msg));
				msg->action = VIEWMSG;
				strcpy(msg->name, str);

				write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"offmsg") == 0)
			{
			    printf("***发送离线消息***\n");
				bzero(msg, sizeof(Msg));
				printf("请输入您要发离线消息的对象:\n");
				scanf("%s",msg->toname);
				printf("请输入离线消息的内容:\n");
				scanf("%s",msg->msg);
				strcpy(msg->name, str);
				msg->action = OFFLINEMSG;

				write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"exit") == 0)
			{
			    printf("\t\t正在退出......\n");
			    bzero(msg, sizeof(Msg));
				msg->action = OFFLINE;
				strcpy(msg->name, str);

				write(sfd, msg, sizeof(Msg));
				exit(1);
			}
	    else if(strcmp(menu,"out") == 0)
			{
			    bzero(msg, sizeof(Msg));
				printf("请输入您要踢出人的姓名:\n");
				scanf("%s",msg->name);
				msg->action = OUT;
                write(sfd, msg, sizeof(Msg));

				printf("%s即将从聊天室踢出!\n",msg->name);
			}
	    else if(strcmp(menu,"ban") == 0)
			{
			    bzero(msg, sizeof(Msg));
				printf("请输入您要禁言人的姓名:\n");
				scanf("%s",msg->name);
				msg->action = BAN;
				write(sfd, msg, sizeof(Msg));

			}
	    else if(strcmp(menu,"rmban") == 0)
			{
			    bzero(msg, sizeof(Msg));
				printf("请输入您要解禁人的姓名:\n");
				scanf("%s",msg->name);
				msg->action = RMBAN;

				write(sfd, msg, sizeof(Msg));
			}
		else
			{
			    printf("\t\tPlease input legal order!\n");
			}
    }
    close(sfd);
    return 0;
}
