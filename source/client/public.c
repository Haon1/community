/**
 *	Community - Client - public_function
 */

#include "../../header/client.h"


/*
	Scan:控制输入,以保正确
*/
int Scan()
{
	int value;
	while(1)
	{
		int retval = scanf("%d",&value);
		
		if(retval==1 && value>=0 && value<=5 && getchar()=='\n')
		{
			return value;
		}
		else printf("bad input!\n");
		
		while(getchar()!='\n');
	}
}

int now_y = 0;
#ifdef ARM
/*
	show_boardcast:在lcd上显示bc_msg 公告
	@bc_msg：公告内容
*/
void show_boardcast(char *bc_msg)
{
	int len = strlen(bc_msg);
	
	fontSetSize(tp, 50);
	
	bp = createBitmapWithInit(500, 300, 4,0xffffffff);

	fontPrint(tp, bp, 0, 0, bc_msg, getColor(0,0,0,255), 500); 

	show_font_to_lcd(devp->map, 0, 0, bp);
}

/*
	show_msg:在公屏上显示聊天消息
	@msg:消息内容
*/
void show_msg(char *msg)
{
	if(now_y == 0)	//该刷新了
	{
		clean_src(0x0080ff,300,480,500,0);
	}
	
	fontSetSize(tp, 20);
	
	bp = createBitmapWithInit(300, 60, 4,getColor(0,255,128,0));

	fontPrint(tp, bp, 5, 0, msg, getColor(0,255,255,255), 300); 

	show_font_to_lcd(devp->map, 500, now_y, bp);
	
	now_y += 60;
	if(now_y == 480)
	{
		now_y = 0;
	}
}

/*
	show_weather:在公屏上显示天气
	@weather：天气内容
*/
void show_weather(char *weather)
{
	fontSetSize(tp, 22);
	
	bp = createBitmapWithInit(500, 180, 4,getColor(0,0,255,0));

	fontPrint(tp, bp, 10, 10, weather, getColor(0,0,0,0), 500); 

	show_font_to_lcd(devp->map, 0, 300, bp);
}

#endif

//清屏 升级版  可指定位置与大小
/*
	clean_src:清屏
	@color:颜色
	@width:宽
	@height:高
	@x_offset:从当前x开始清屏
	@y_offset:从当前y开始清屏
*/
void clean_src(int color ,int width,int height,int x_offset,int y_offset)
{
	
	int x,y;
	
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			devp->map[(x+x_offset)+(y+y_offset)*800]=color;
		}
	}
}


/*
	get_time：获取当前系统时间
*/
struct tm *get_time()
{
	time_t T;
	time(&T);
	struct tm *time = gmtime(&T);
	
	return time;
	
}

/*
	mystrcat:字符串拼接,\0不丢失
	@dest：拼接到哪个字符串后
	@src：要拼接的字符串
	@num：要拼多少个字节
*/
char *mystrcat(char *dest,const char *src,int num)
{
	while(*dest != '\0')
	{
		dest++;
	}
	
	while(num != 0)
	{
		*dest = *src;
		dest++;
		src++;
		num--;
	}
	
	return dest;
}

/*
	format:判断文件文件是否为指定格式
	@file:文件名
	@fm:格式后缀名
*/
bool get_format(const char *file,const char *fm)
{
	int f_len = strlen(file);
	int m_len = strlen(fm);
	int i = 0;
	
	for(i=0;i>m_len;i--)
	{
		if(file[f_len-i-1] != fm[m_len -i -1])
		{
			return false;
		}
	}
	
	return true;
}