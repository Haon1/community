/**
 *	Community - Client - MSG_function
 */

#include "../../header/client.h"


/*
	sndto_serv:给服务器发送消息
	@socket:客户端自己的socket
*/
void sndto_serv(int socket)
{
	printf("输入消息内容,over结束\n");
			
	while(1)
	{
		char tmp[1000]={0};
		char msg[1024]={0};
		
		scanf("%s",tmp);
		if(strcmp(tmp,"over")==0)
		{
			break;
		}
		sprintf(msg,"%d@%s",CHAT,tmp);		//CHAT
		
		write(socket,msg,strlen(msg));
	}
}

/*
	sndto_serv:区友发送消息
	@socket:客户端自己的socket
*/
void sndto_clit(int socket)
{	
	char name[50]={0};
	printf("发谁：");
	scanf("%s",name);
	printf("开始吧,over结束\n");
	while(1)
	{
		
		char tmp[4000]={0};
		char msg[4096]={0};
		scanf("%s",tmp);
		
		if(strcmp(tmp,"over")==0)
		{
			break;
		}
		sprintf(msg,"%d@%s@%s",THRANSPOND,name,tmp);
		write(socket,msg,strlen(msg));
	}
}

/*
	sndto_file:给色区好友发送学习资料
	@socket：自己的socket
*/
void sndto_file(int socket)
{
	char dest_name[50]={0};
	char file[100]={0};		//文件名
	
	printf("发谁:");
	scanf("%s",dest_name);
	printf("要发送的文件：");
	scanf("%s",file);
	
	int fd = open(file,O_RDWR);
		if(fd < 0)
		{
			perror("file open err");
			return ;
		}
	
	int filesize = get_filesize(file);
	
	//定制SYN消息,与接收方建立通信
	char msg[1024]={0};
	
	sprintf(msg,"%d@%s@%s#%d",SYN,dest_name,file,filesize);	
	
	write(socket,msg,strlen(msg));	//发送SYN信息给服务器处理,转发给接收方
	
	//阻塞等待接收方回应
	while(response!= ACCEPT);
	
	//开始传输
	while(1)
	{
		char buf[1024]={0};
		char file_data[1000]={0};
		
		int size = read(fd,file_data,1000);
			if(size == 0)
			{
				printf("发送完毕\n");
				break;
			}
		//printf("read size = %d\n",size);	//读出来4000字节
		//printf("file_data = %s\n",file_data);
		
		sprintf(buf,"%d@%s@",FILEE,dest_name);
		mystrcat(buf,file_data,size);			//把文件数据追加到消息后面，不能用strcat,也不能sprintf
												//因为文件中会有 值为0的数据， 遇到0后,之后的数据都会丢失
		
		int wsize = write(socket,buf,3+strlen(dest_name)+size);
	//	printf("wsize = %d\n",wsize);			//打印实际写入大小
		//printf("write  = %s\n",buf);
		response = RESET;						//发完一段,阻塞等待接受方确认接收完毕
		while(response != ACCEPT);				//再开始发下一段

	}
	
	close(fd);
	response = RESET;	//重置
}


/*
	get_filesize:获取一个文件的大小
	@file:文件的路径
*/
int get_filesize(char *file)
{
	int fd = open(file,O_RDWR);
	if(fd < 0)
	{
		perror("");
		return 0;
	}
	
	int size = lseek(fd,0,SEEK_END);
	
	close(fd);
	
	return size;
}

/*
	rcv_file:接收文件
	@socket：自己的socket
	@selfname:自己的名字
	@msg：对方发来的msg
*/
void rcv_file(int socket,char *msg)
{
	char *first = strstr(msg,"@");		//第一个@
	char *second = strrchr(msg,'@');	//第二个@
	char *size = strstr(msg,"#");		// "#"
	
	
	char snd_name[50]={0};				//发送人
	strncpy(snd_name,first+1,(int)(second - first - 1));	//提取名字

	
	char file_name[50]={0};				//文件名
	strncpy(file_name,second+1,(int)(size - second -1));	//提取文件名

	int filesize = atoi(size + 1);							//提取文件大小
	
	printf("%s给你发来一个文件%s,%dbytes\n",snd_name,file_name,filesize);
	
	//回应发送方已经收到大小
	char res[1024]={0};
	sprintf(res,"%d@%s@%d",RESPONSE,snd_name,ACCEPT);
	write(socket,res,strlen(res));
	
	//创建文件
	int fd = open(file_name,O_RDWR|O_CREAT|O_TRUNC,0777);
	if(fd < 0)
	{
		perror("creat file err");
		return ;
	}
	int download = 0;
	
	//开始接收文件
	while(1)
	{
		char buf[4096]={0};
		//char file_data[4096]={0};
		
		int rsize = read(socket,buf,4096);
	//	printf("r = %d\n",rsize);
	//	printf("buf = %s\n",buf);
	
		//提取类型
		int type = atoi(buf);
		//printf("rcvfile type = %d\n",type);
		
		if(type == FILEE)
		{
			char *tmp=strstr(buf,"@");
			int wsize = write(fd,tmp+1,rsize-2);
			download += wsize;
			
			//printf("wsize = %d\n",wsize);
			//printf("写进 %s\n",tmp+1);
			
			printf("下载进度%d%c\n",(int)((float)download/filesize*100),37);
			
			//sleep(1);
			//回应这一段已经收到可以开始下一段
			sprintf(res,"%d@%s@%d",RESPONSE,snd_name,ACCEPT);
			write(socket,res,strlen(res));
			
			
			if(download == filesize)
			{
				printf("下载完毕\n");
				break;
			}
		}
		
	}
	
	close(fd);
	
	#ifdef ARM
	if(get_format(file_name,".bmp"))
	{
		clean_src(0xffffff,800,480,0,0);	//清屏
		display(file_name);
	}
	#endif
	
}