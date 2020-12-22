/**
 *	Community - Server - thread_handler
 */


#include "../../header/server.h"

/*
	manage：管理入口
*/
void *manage(void *arg)
{
	while(1)
	{
		printf("1.在线列表  2.发消息 3.征婚广播\n");
		
		int val = Scan();
		
		if(val == 1)	
		{
			show_client();		//列表
		}
		else if(val == 2)
		{
			printf("请输入客户序号\n");		//发消息
			scanf("%d",&val);
			struct client *cp = get_client_bycode(val);
			
			if(cp)
			{
				printf("开始吧,over结束\n");
				while(1)
				{
					char tmp[1000]={0};
					char msg[1024]={0};
					
					scanf("%s",tmp);
					if(strcmp(tmp,"over")==0)
					{
						break;
					}
					sprintf(msg,"%d@%s",CHAT,tmp);
				
					write(cp->socket,msg,strlen(msg));
				}
			}
			else printf("不在线\n");
			
			
		}
		else if(val == 3)		//公告
		{
			printf("输入征婚内容\n");
			char tmp[1000]={0};
			scanf("%s",tmp);	//输入公告
			sprintf(boardcast_msg,"%d@%s",BOARDCAST,tmp);
			
			pthread_cond_signal(&cond);	//唤醒线程发送公告
		}
	}
}

/*
	rcv：读取并处理客户消息
*/
void *rcv(void *arg)
{
	
	//int *p = arg;		//!!!!!!!这样的话可能 arg指向还在同一地方，但是socket的值被第二个客户覆盖了\
						所以前一个线程读到的会是 第二个 socket的内容，读不到自己的数据，靠tmd！！
						
	int socket = *((int *)arg);		//这样写取值就稳了
	
	//通过socket 获取客户ip信息
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);
	getpeername(socket,(struct sockaddr *)&addr,&addrlen);		//不用的话不知道用户地址
	
	
	//处理连接信息
	char login[1024]={0};
	read(socket,login,1024);
	char this_name[50]={0};
	sscanf(login,"login %s",this_name);
	add_client(this_name,socket,addr);
	recode();
	people++;
	
	struct client *p = get_client_bysock(socket);	//通过socket获取该客户信息
													//不用的话不知道名字
	printf("\n[%d] %s -->fd:%d  ip:%s  port:%d  接入\n\n",people,p->name,socket,\
														inet_ntoa(p->addr.sin_addr),\
														ntohs(p->addr.sin_port));
	
	//读取消息
	while(1)
	{
		char msg[4096]={0};
		
		int size = read(socket,msg,4096);
			if(size == 0)
			{	
				printf("%s ip:%s 掉线\n",p->name,inet_ntoa(p->addr.sin_addr));
				
				close(socket);											
				del_client(socket);
				recode();
				pthread_exit(NULL);
			}
		
		int type = atoi(msg);		//提出消息类型
		//int type;
		//sscanf(msg,"%d@",type);
		//printf("type = %d\n",type);
		//printf("read size = %d\n",size);
		
		if(type == CHAT)	//1 普通聊天(客户端和服务器之间)	type@msg
		{
			char *text = strstr(msg,"@")+1;
			printf("%s %s: %s\n",p->name,inet_ntoa(p->addr.sin_addr),text);
		}
		else if(type == THRANSPOND)	//2 转发消息(cli to cli )	type@destname@msg
		{
			char *first = strstr(msg,"@");		//第一个@
			char *second = strstr(first+1,"@");	//第二个@

			char dest_name[100]={0};
			strncpy(dest_name,first+1,(int)(second-first)- 1);	//提取接收方名字
			
			char *text = second+1;		//拿出消息正文
			
			printf("%s要发给%s\n",p->name,dest_name);
			struct client *rcv_client = get_client_byname(dest_name);	//获取目的socket
			
			char trans[1024]={0};
			sprintf(trans,"%d@%s@%s",THRANSPOND,p->name,text);
			write(rcv_client->socket,trans,strlen(trans));					//转发
		}
		else if(type == GET)		//3 要获取列表	type
		{
			char *list = get_list();	//获取本地列表,get里面申请堆空间
			char get[1024]={0};
			sprintf(get,"%d@%s",GET,list);
			
			write(socket,get,strlen(get));	//发给客户端
			
			free(list);
		}
		else if(type == SYN)		//4 同步 type@destname@filename#filesize
		{
			char *first = strstr(msg,"@");		//第一个@
			char *second = strstr(first+1,"@");	//第二个@
			char *fsize = strstr(msg,"#");		// "#"
			
			char dest_name[100]={0};
			strncpy(dest_name,first+1,(int)(second-first)- 1);	//提取接收方名字
			
			char file_name[50]={0};				//文件名
			strncpy(file_name,second+1,(int)(fsize - second -1));	//提取文件名
			strcpy(file_name,strrchr(file_name,'/')+1);
			
			int filesize = atoi(fsize + 1);							//提取文件大小
			
			struct client *rcv_client = get_client_byname(dest_name);
			
			char syn[1024]={0};
			sprintf(syn,"%d@%s@%s#%d",SYN,this_name,file_name,filesize);
			
			printf("%s要给%s发一个文件,%s %d字节\n",this_name,dest_name,file_name,filesize);
			
			write(rcv_client->socket,syn,strlen(syn));		//转发
		}
		else if(type == RESPONSE)	//5 回应	type@dest_name@r_msg
		{
			char *first = strstr(msg,"@");		//第一个@
			char *second = strstr(first+1,"@");	//第二个@
			
			char dest_name[100]={0};
			strncpy(dest_name,first+1,(int)(second-first)- 1);	//提取接收方名字
			
			struct client *rcv_client = get_client_byname(dest_name);
			
			char resp[1024]={0};
			sprintf(resp,"%d@%s@%d",RESPONSE,this_name,ACCEPT);
			write(rcv_client->socket,resp,strlen(resp));
		}
		else if(type == FILEE)		//6 文件传输 type@destname@file_data
		{
			//printf("发送方发来 ---%s\n",msg);
			char *first = strstr(msg,"@");		//第一个@
			char *second = strstr(first+1,"@");	//第二个@
			
			char dest_name[100]={0};
			int r_type=0;
			//sscanf(msg,"%d@%[^@]@",&r_type,dest_name);
			strncpy(dest_name,first+1,(int)(second-first)- 1);	//提取接收方名字
		//	printf("dest_name = %s\n",dest_name);
			
			struct client *rcv_client = get_client_byname(dest_name);	//获取接收方socket

			//printf("文件内容%s\n",second+1);
			char file_data[4096]={0};
			sprintf(file_data,"%d@",FILEE);		//去掉名字  以 type@file_data 形式发给接收方
			mystrcat(file_data,second+1,size - strlen(dest_name)-3);
			
			int send_size=size-strlen(dest_name)-1;
			//printf("发送大小=%d\n",send_size);
			write(rcv_client->socket,file_data,send_size);	//2?????
			
		}
		
	}
}

/*
	boardcast:在小区广播征婚消息
*/
void *boardcast(void *arg)
{
	while(1)
	{
		//线程进入休眠等待任务
		pthread_cond_wait(&cond,&lock);
		
		//推送
		putmsg(boardcast_msg);
	}
}

