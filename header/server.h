#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "kernel_list.h"

/*		消息类型	*/
#define BOARDCAST 	0x00		/*		广播		*/
#define CHAT		0x01		/*		聊天信息	*/
#define THRANSPOND	0x02		/*		转发		*/
#define GET			0x03		/*		获取列表	*/
#define SYN			0x04		/*		同步		*/
#define RESPONSE	0x05		/*		回应		*/
#define FILEE		0x06		/*		文件发送	*/
/*
	BOARDCAST -->    type@bc_msg
	CHAT -->		 type@c_msg
	THRANSPOND -->	 type@destname@t_msg
	GET  -->		 type
	FILEE -->		 type@destname@file_data
	RESPONSE  -->	 type@destname@r_msg
	SYN -->			 type@destname@filename#filesize
*/

#define RESET		0x00		/*	重置	*/
#define ACCEPT		0x01		/*	接收	*/
#define REFUSE		0x02		/*	拒绝	*/

struct client
{
	char name[20];
	int code;						/*	序号			*/
	int socket;						/*	socket 描述符	*/
	struct sockaddr_in addr;		/*	客户地址		*/
	
	struct list_head list;			/*	链表			*/
};


extern struct list_head *head;	//客户端链表头

//extern font *tp;       //字体指针
//extern bitmap *bp;     //字体背景指针

extern int people;		//当前连接数

extern pthread_mutex_t lock;	//线程锁
extern pthread_cond_t cond;		//条件变量

extern char boardcast_msg[1024];		//公告内容


/*			函数声明			*/
void *manage(void *arg);
void *rcv(void *arg);
void *boardcast(void *arg);


struct client *init_client(char *name,int socket,struct sockaddr_in addr);
void add_client(char *name,int socket,struct sockaddr_in addr);
void del_client(int socket);
void show_client();
void recode();

struct client *get_client_bycode(int code);		//序号获取，主动发需要
struct client *get_client_byname(char *name);	//通过名字获取，转发时获取目标客户需要
struct client *get_client_bysock(int socket);	//通过socket获取，转发获取源客户需要

void putmsg(char *msg);
char *get_list();


int Scan();
char *mystrcat(char *dest,const char *src,int num);




#endif