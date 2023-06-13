#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/select.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "fun.h"


#define RET_OK 	0
#define RET_ERR	-1
#define LISTEN_QUEUE_NUM 5
#define BUFFER_SIZE 1024
#define ECHO_PORT 2029

int q;


/*功能：用户登录
参数：sockfd	 	客户端TCP套接字
      p_onlinefd	服务器TCP连接后套接字的指针
      username		登录名
      onlinename 	在线用户列表
返回值：成功返回0；失败返回-1*/

int client_login (int sockfd, int *p_onlinefd, char *username, char *onlinename)
{
	struct user me={"q","q",0};	
	struct user rgist={"q","q",1};	 //保存注册信息的结构体;	
	int flag = 0;		//1是成功，0是失败。
	int flag1=0;
	int a=0;
	char code[20];

	while(1)
	{
		if(flag1==1){
			break;
		}
		system("clear");
		system("tput cup 1 3"); printf("\t\033[34;1m********************************************************\n");	
		system("tput cup 2 3"); printf("\t*             聊 天 系 统 v1.0             *\n");
		system("tput cup 3 3"); printf("\t*------------------------------------------------------*\n");
		system("tput cup 4 3"); printf("\t*             1、登  陆                                *\n");
		system("tput cup 5 3"); printf("\t*------------------------------------------------------*\n");
		system("tput cup 6 3"); printf("\t*             2、注  册                                *\n");
		system("tput cup 7 3"); printf("\t*------------------------------------------------------*\n");
		system("tput cup 8 3"); printf("\t*             3、退  出                                *\n");
		system("tput cup 9 3"); printf("\t*------------------------------------------------------*\n");
		system("tput cup 10 3"); printf("\t*             请  选  择:                              *\n");
		system("tput cup 11 3"); printf("\t********************************************************\n");
		system("tput cup 10 34");
		scanf("%d",&a);
		setbuf(stdin,NULL);
		//getchar();
		switch(a)
		{
			case 1:
				system("clear");
				system("tput cup 1 3"); printf("\t\033[34;1m********************************************************\n");	
				system("tput cup 2 3"); printf("\t*               请    登   录                          *\n");
				system("tput cup 3 3"); printf("\t*------------------------------------------------------*\n");
				system("tput cup 4 3"); printf("\t*             用 户 名:                                *\n");
				system("tput cup 5 3"); printf("\t*------------------------------------------------------*\n");
				system("tput cup 6 3"); printf("\t*             密 码:                                   *\n");
				system("tput cup 7 3"); printf("\t********************************************************\n");
				system("tput cup 4 34");
				scanf("%s",me.name);
				//printf("%s\n",me.name);
				setbuf(stdin,NULL);
				system("stty -echo");
				system("tput cup 6 34");
				scanf("%s",me.code);
				//printf("%s\n",me.code);
				setbuf(stdin,NULL);
				system("stty echo");
				system("tput cup 8 3");  printf("\t正在登录,请稍等...\033[0m\n");

				send(sockfd, &me, sizeof(struct user), 0);	//发送用户名和密码
				recv(sockfd, &flag, sizeof(int), 0);		//接收服务器的信息flag

				if(flag == 1)//验证成功
				{
					strcpy(username, me.name);

					recv(sockfd, p_onlinefd, sizeof(int), 0);   
					flag1=1;
					sleep(1);
					//break;
				}
				else if(flag == -1)//重复登录
				{
					printf("\033[34;1m该用户已经登录!\033[0m\n");
					sleep(2);
				}
				else	//验证失败
				{
					system("tput cup 10 3");  printf("\033[34;1m您的用户名或密码有误,请重新登录!\033[0m\n");
					system("tput cup 11 3");  printf("\033[34;1m2s后将返回首界面.\033[0m\n");
					sleep(2); 
				}
				break;
			case 2:
					system("clear");
				system("tput cup 1 3"); printf("\t\033[34;1m********************************************************\n");	
					system("tput cup 2 3"); printf("\t*               请    注   册                          *\n");
					system("tput cup 3 3"); printf("\t*------------------------------------------------------*\n");
					system("tput cup 4 3"); printf("\t*             用 户 名:                                *\n");
					system("tput cup 5 3"); printf("\t*------------------------------------------------------*\n");
					system("tput cup 6 3"); printf("\t*             密 码:                                   *\n");
					system("tput cup 7 3"); printf("\t*------------------------------------------------------*\n");
					system("tput cup 8 3"); printf("\t*             确 认 密 码:                             *\n");
					system("tput cup 9 3"); printf("\t********************************************************\n");
					system("tput cup 4 34");
					scanf("%s",rgist.name);
					setbuf(stdin,NULL);
					system("stty -echo");
					system("tput cup 6 30");
					scanf("%s",rgist.code);
					setbuf(stdin,NULL);
					system("stty echo");
					system("stty -echo");
					system("tput cup 8 34");
					scanf("%s",code);
					setbuf(stdin,NULL);
					system("stty echo");
					if(strcmp(rgist.code,code)==0)
					{
						send(sockfd, &rgist, sizeof(struct user), 0);	//发送用户名和密码
						recv(sockfd, &flag, sizeof(int), 0);		//接收服务器的信息flag
						//printf("flag=%d\n",flag);
						if(flag == 1)//注册成功;
						{
							system("tput cup 12 3"); printf("注册成功!\n");
							sleep(1);
							break;
						}
						else //用户重复注册;
						{
							system("tput cup 12 3"); printf("\033[34;1m该用户已被注册!\033[0m\n");
							//sleep(2);
						}
					}
					else{
						system("tput cup 12 3"); printf("两次密码不一致!即将返回主界面!\n");
					}
				sleep(2);
				break;
			case 3:
				kill(q+2,SIGKILL);
        			kill(q+3,SIGKILL);
				kill(q+4,SIGKILL);
				exit(0);
				break;
			default:
				break;
		}
	}

	system("tput cup 0 3");  printf("\033[32;1m登录成功\033[0m\n");
	system("reset");
	system("tput cup 1 3");  printf("\t\t\033[34;1m“%s” \033[0m\033[32;1m你好!\033[0m\n",username);
	system("tput cup 3 3");  printf("\033[32;1m--------------------使用说明--------------------\n");
	system("tput cup 4 3");  printf("私人聊天模式->格式为 聊天对象名:\033[0m\033[34;1m聊天内容\033[0m\n");
	system("tput cup 5 3");  printf("\033[32;1m多人聊天模式->格式为 对象名:对象名:\033[0m\033[34;1m聊天内容\033[0m\n");
	system("tput cup 6 3");  printf("\033[32;1m群聊聊天模式->格式为 all:\033[0m\033[34;1m聊天内容\033[0m\n");
	system("tput cup 7 3");  printf("\033[32;1m退出聊天－>  \033[0m\033[34;1mquit\033[0m\n");
	system("tput cup 9 3");  printf("\033[32;1m------------------------------------------------\033[0m\n");
	system("tput cup 8 3");  printf("开始你的聊天吧，祝您聊的愉快...\033[0m\n\n");

	recv(sockfd, onlinename, 1024, 0);	

	return 0;
}


int main (int argc, char *argv[])
{
	int i=0;
	

	int sock;			//套接字
	struct sockaddr_in servaddr;
	//struct hostent *server;	
	int len;	
	char buf[BUFFER_SIZE];	
	
	int onlinefd;		//服务器端的TCP连接后的套接字
	char username[20];	//用户名
	char onlinename[1024];	//在线用户列表

	struct msg msgs;	

	struct tm *ptm;		//时间有关
	time_t ts;
	char tim[50];

	
	//创建socket套接字
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return -1;
	}
	
	//设置IP 和 端口号
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//servaddr.sin_port = htons(ECHO_PORT); 
	servaddr.sin_addr.s_addr = inet_addr("192.168.31.130");//指定IP
	servaddr.sin_port = htons(8001); 
	
	//连接服务器
	if(connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect");
		return -1;
	}
	pid_t pid=fork();

	if(pid<0)
	{
		fprintf(stderr,"错误!");
	}
	else if(pid==0){
		exit(0);
		}
		else
		{
			q=pid+1;
			printf("%d\n",q);
		}
	//用户登录
	if(client_login(sock, &onlinefd, username, onlinename) != 0)
	{
		close(sock);
		return -1;
	}
	system("tput cup 10 3");  printf("\033[34;1m%s\033[0m\n", onlinename);	//打印在线用户名单

	//添加进程
	if((pid = fork()) < 0)
	{
		return -1;
	}

	if(pid == 0)	//该子进程接收消息（一般是其他用户的服务器处理程序 发送过来的）
	{
		while(1)
		{
			bzero(&msgs, sizeof(msgs));
			len = recv(sock, &msgs, sizeof(msgs), 0);

			if(len > 0)
			{	
				printf("%s", msgs.info);
				printf("\033[34;1m%s\033[0m", msgs.message);
				sleep(1);	
				printf("---------------------------------------------------------");
			}
			else if(len < 0)
			{
				perror("recv : ");
				break;
			}
			else
			{
				system("tput cup 10 3");  printf("\n\033[31;1m对不起，服务器出问题了\n");
				system("tput cup 11 3");  printf("我们正在抢修，很抱歉给您造成麻烦了\033[0m\n");

				//结束父进程
				kill(getppid(), SIGKILL);
				break;				
			}

		}//end wile	
	}	
	else	//父进程发送消息（只向该用户的服务器处理程序 发送）	
	{

		while(1)
		{
			bzero(buf, BUFFER_SIZE);
			bzero(&msgs, sizeof(struct msg));

			msgs.fd = onlinefd;
			strcpy(msgs.from_name, username);

			if(fgets(buf, BUFFER_SIZE, stdin) == NULL)
			{
				if(ferror(stdin))
				{
					perror("stdin");
					break;
				}
			}
			strcpy(msgs.message, buf);			
			//判断是否退出
			if(!strncasecmp(buf, "quit", 4))
			{
				send(sock, &msgs, sizeof(msgs), 0);
				system("tput cup 4 13");  printf("\033[31;1m程序即将退出！再见!\033[0m\n");
				sleep(2);
				system("clear");
				kill(0,SIGKILL);
				//exit(0);
				break;
			}
			if(!strncasecmp(buf,"stop",4))
			{
			
				kill(q+2,SIGKILL);
				kill(q+3,SIGKILL);
				kill(q+4,SIGKILL);
			}			
		
			ts = time(NULL);			//格式化时间
			ptm = localtime(&ts);

			sprintf(tim, "(%d-%d-%d %02d:%02d:%02d):", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
			sprintf(msgs.info, "\n%s %s", username, tim);
			if(strncasecmp(buf, "all:习近平", 8))	
			{	
				if(send(sock, &msgs, sizeof(msgs), 0) < 0)
				{
					printf("send ! %s", strerror(errno));
					break;
				}
			}else
			{
				printf("\033[31;1m<习近平>属于敏感词汇!本次对话不能发送!三次后自动退出程序!请谅解!\033[0m\n");
				i++;
			}
			if(i>=3)
			{
				printf("\033[31;1m您发送的语句中敏感词已超过3次,2s后程序关闭!\n\033[0m");
				sleep(2);
				system("clear");
				kill(q+2,SIGKILL);
				exit(0);
			}

		}//end while
		
		//结束子进程	
		kill(pid, SIGKILL);	
	}//end if

	close(sock);	
	
	return 0;
}

