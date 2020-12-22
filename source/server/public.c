/**
 *	Community - Server - public_function
 */

#include "../../header/server.h"

/*
	Scan:控制输入,以保证却
*/
int Scan()
{
	int value;
	while(1)
	{
		int retval = scanf("%d",&value);
		
		if(retval==1 && value>=1 && value<=3 && getchar()=='\n')
		{
			return value;
		}
		else printf("bad input!\n");
		
		while(getchar()!='\n');
	}
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
