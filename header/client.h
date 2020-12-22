#ifndef _CLIENT_H
#define _CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#include <linux/input.h>

#include "font.h"
#include "kernel_list.h"

#define ARM

#define LCD_SIZE 800*480*4

#define L		4		//左滑
#define R		6		//右滑
#define U		8		//上滑
#define D	 	2		//下滑
#define N 		5		//点击

/*		消息类型	*/
#define BOARDCAST 	0x00		/*		广播		*/
#define CHAT		0x01		/*		聊天信息	*/
#define THRANSPOND	0x02		/*		转发		*/
#define GET			0x03		/*		获取列表	*/
#define SYN			0x04		/*		请求同步		*/
#define RESPONSE	0x05		/*		回应		*/
#define FILEE		0x06		/*		文件发送	*/
/*
	BOARDCAST -->    type@bc_msg
	CHAT -->		 type@c_msg
	THRANSPOND -->	 type@dest_scoket@t_msg
	GET  -->		 type@list
	FILEE -->		 type@destname@file_data
	RESPONSE  -->	 type@destname@r_msg
	SYN -->			 type@destname@filename#filesize
*/

#define RESET		0x00		/*	重置	*/
#define ACCEPT		0x01		/*	接收	*/



struct dev
{
	int lcd_fd;
	int *map;
};


/*	全局变量	*/
extern font *tp;      			 //字体指针
extern bitmap *bp;    			 //字体背景指针
extern struct dev *devp;
extern int response;			//接收文件状态

extern char now_wther[1024];	//保存当前的广播消息
extern int now_y;				//文本框当前y偏移量

/*	函数声明	*/
void init_dev();

void display(char *filename);


void *rcv(void *arg);

int Scan();
void show_boardcast(char *bc_msg);
void show_weather(char *weather);
void show_msg(char *msg);

void clean_src(int color ,int width,int height,int x_offset,int y_offset);
struct tm *get_time();
char *mystrcat(char *dest,const char *src,int num);
bool get_format(const char *file,const char *fm);


void sndto_serv(int socket);
void sndto_clit(int socket);
void sndto_file(int socket);

int get_filesize(char *file);
void rcv_file(int socket,char *msg);

void toget_weather(int socket,char *city);



#endif