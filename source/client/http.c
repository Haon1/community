#include "../../header/client.h"

char now_wther[1024]={0};

/*
	toget_weather:通过http获取天气,并输出在屏幕上
	@socket:连接http服务器的socket
	@city: 要获取的城市
*/
void toget_weather(int socket,char *city)
{
	//http://api.qingyunke.com/api.php?key=free&appid=0&msg=天气广州
	
	//制定http 请求
	char http[1024]={0};
	
	sprintf(http,"GET /api.php?key=free&appid=0&msg=天气%s HTTP/1.1\r\nHost:api.qingyunke.com\r\n\r\n",city);
	
	write(socket,http,strlen(http));	//发送 http 请求
	
	char buf[4096]={0};
	read(socket,buf,4096);
	
	
	//偏移到正文
	char *text = strstr(buf,city);    	//"城市天气 {br}[12月20日]:..."
	
	char *p = NULL;
	while((p = strstr(text,"{br}"))!=NULL)
	{
		strcpy(p+1,p+4);
		*p = '\n';
		p+=4;
	}
	p = strrchr(text,'"');
	*p = '\0';
	
	printf("text = %s\n",text);
	
	strcpy(now_wther,text);		//保存一下当前天气文本
	
	#ifdef ARM
	show_weather(text);
	#endif
}