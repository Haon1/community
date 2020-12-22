/**
 *	Community - Server
 */

#include "../../header/server.h"


int people = 0;					//当前连接服务端数量
struct list_head *head = NULL;	//链表头

char boardcast_msg[1024]={0};		//公告内容

pthread_mutex_t lock;		//线程锁
pthread_cond_t cond;		//条件变量


int main(int argc, char *argv[])
{
	//申请堆空间并初始化
	head = (struct list_head *)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(head);
	
	//初始化锁和条件变量
	pthread_mutex_init(&lock,NULL);
	pthread_cond_init(&cond,NULL);
	
	
	//创建 socket
	int server_socket = socket(AF_INET,SOCK_STREAM,0);
	
	if(server_socket < 0)
	{
		fprintf(stderr,"%s","socket err");
		perror("");
		return 0;
	}
	
	//设置端口复用
	int on = 1;
	setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	
	//设置服务器 ip 信息
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	//绑定信息
	int retval = bind(server_socket,(struct sockaddr *)&server_addr,sizeof(server_addr));
	
	if(retval < 0)
	{
		fprintf(stderr,"%s","bind err");
		perror("");
		
		close(server_socket);
		return 0;
	}
	
	//监听
	retval = listen(server_socket,1023);
	
	if(retval < 0)
	{
		fprintf(stderr,"%s","listen err");
		perror("");
		
		close(server_socket);
		return 0;
	}
	
	//开线程去处理任务
	pthread_t tid1, tid2;
	pthread_create(&tid1,NULL,manage,NULL);		//列表,发消息这些
	pthread_create(&tid2,NULL,boardcast,NULL);	//发公告
	pthread_detach(tid1);
	pthread_detach(tid2);
	
	printf("等待客户端连接...\n");
	
	
	//等待客户端连接
	while(1)
	{
		int client_socket = accept(server_socket,NULL,NULL);
		
		if(client_socket > 0)
		{
			pthread_t tid;
			pthread_create(&tid,NULL,rcv,&client_socket);
			pthread_detach(tid);
		}
		else printf("连接失败\n");
		
	}
	
	
}