/**
 *	Community - Client -thread_handler
 */

#include "../../header/client.h"

char now_bcast[1024]={0};	//保存当前的广播消息

/*
	rcv:读取服务器消息并处理
*/
void *rcv(void *arg)
{
	int socket = *((int *)arg);
	
	while(1)
	{
		char msg[1024]={0};
		
		int size = read(socket,msg,1024);		
			if(size == 0)
			{
				printf("服务器断开连接...\n");
				close(socket);
				exit(0);
				//pthread_exit(NULL);
			}
		
		//取出消息类型
		int type = atoi(msg);
		//printf("type = %d\n",type);
		
		if(type == BOARDCAST)	//如果是广播	type@bc_msg
		{
			char *bc_msg = strstr(msg,"@")+1;
			strcpy(now_bcast,bc_msg);
			printf("bc_msg = %s\n",bc_msg);
		#ifdef ARM
			show_boardcast(bc_msg);		//显示公告在屏幕上
		#endif
		}
		else if(type == CHAT)	//普通聊天		type@c_msg
		{
			char *text = strstr(msg,"@")+1;
			printf("server: %s\n",text);
			char tmp[1024]={0};
			sprintf(tmp,"server: %s",text);
			#ifdef ARM
			show_msg(tmp);
			#endif
		}
		else if(type == THRANSPOND)	//和好友聊天	type@dest_scoket@t_msg
		{
			char *first = strstr(msg,"@");		//第一个@
			char *second = strrchr(msg,'@');	//第二个@
			char fri_name[50]={0};
			strncpy(fri_name,first+1,(int)(second - first - 1));	//提取名字
			
			char *text = second+1;		//拿出消息正文

			printf("%s:%s\n",fri_name,text);
			
			char tmp[1024]={0};
			sprintf(tmp,"%s: %s",fri_name,text);
			#ifdef ARM
			show_msg(tmp);
			#endif
		}
		else if(type == GET)		//返回的是列表	GET@列表
		{
			char *list = strstr(msg,"@")+1;			//取出列表
			system("clear");
			printf("--------在线区友---------\n%s\n",list);
		}
		else if(type == SYN)		//收到文件		type@src_name@filename#filesize
		{
			rcv_file(socket,msg);
			sleep(3);		//显示3秒图片
			
			#ifdef ARM
			show_boardcast(now_bcast);		//显示公告
			show_weather(now_wther);		//显示天气
			now_y = 0;
			clean_src(0x0080ff,300,480,500,0);
			#endif
		}
		else if(type == RESPONSE)	//文件回应		type@src_tname@r_msg
		{
			response = atoi(strrchr(msg,'@')+1);
			printf("response = %d\n",response);
		}
	}
}
