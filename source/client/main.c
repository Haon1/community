/**
 *	Community - Client
 */

#include "../../header/client.h"

font *tp = NULL;       //字体指针
bitmap *bp = NULL;     //字体背景指针
struct dev *devp =	NULL;	//硬件资源管理器
int response = RESET;	//接收文件状态

int main(int argc,char *argv[])
{
	if(argc != 4)
	{
		printf("参数错误  ./client  服务器ip  服务器端口 你的名字\n");
		return 0;
	}
	
	
	#ifdef ARM
	//初始化硬件资源
	devp = (struct dev *)malloc(sizeof(struct dev));
	init_dev();
	clean_src(0xffffff,500,300,0,0);	//清屏	公告区
	clean_src(0x00ff00,500,180,0,300);	//天气区
	clean_src(0x0080ff,300,480,500,0);	//聊天区域
	#endif
	
	//创建socket
	int client_socket = socket(AF_INET,SOCK_STREAM,0);
		if(client_socket < 0)
		{
			perror("socket err:");
			return 0;
		}
	
	//设置小区服务器属性
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	//连接小区服务器
	int retval = connect(client_socket,(struct sockaddr *)&server_addr,sizeof(server_addr));
	
	if(retval < 0)
	{
		perror("connect community err");
		return 0;
	}

#if 1	
	//http socket
	int http_socket = socket(AF_INET,SOCK_STREAM,0);
		if(http_socket < 0)
		{
			perror("socket err");
			return 0;
		}
	//设置http服务器属性
	struct sockaddr_in http_addr;
	http_addr.sin_family = AF_INET;
	http_addr.sin_port = htons(80);
	http_addr.sin_addr.s_addr = inet_addr("47.107.155.132");
	
	//连接http
	retval = connect(http_socket,(struct sockaddr *)&http_addr,sizeof(http_addr));
	
	if(retval < 0)
	{
		perror("connect http err");
		return 0;
	}
#endif	
	
	//给社区服务器发送上线消息
	char login[50]={0};
	sprintf(login,"login %s",argv[3]);
	write(client_socket,login,strlen(login));

	//开线程去读消息
	pthread_t tid,tid1;
	pthread_create(&tid,NULL,rcv,&client_socket);	//读消息
	pthread_detach(tid);
	
	//发消息？？？
	while(1)
	{
		printf("1.在线列表  2.to服务器  3.to区友 4.转发文件  5.获取天气  0.夯机\n");
		
		int val = Scan();
		
		if(val == 1)				//在线列表
		{
			char type[20]={0};
			sprintf(type,"%d",GET);			//GET
			write(client_socket,type,strlen(type));		//获取一下列表
			usleep(100000);		//让出时间片先打印列表
		}
		else if(val == 2)				//发消息给服务器
		{
			sndto_serv(client_socket);
		}
		else if(val == 3)		//发消息给区友
		{
			sndto_clit(client_socket);
		}
		else if(val == 4)		//转发文件
		{
			sndto_file(client_socket);
		}
		else if(val == 5)		//获取天气
		{
			char city[30]={0};
			char msg[1024]={0};
			printf("输入城市\n");		//输入城市
			scanf("%s",city);
			
			toget_weather(http_socket,city);
		}
		else			//夯机
		{
			close(client_socket);
			exit(0);
		}	
	}
	
}

