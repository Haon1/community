#include "../../header/client.h"


/*
	display:居中显示bmp图片
	@filename:图片路径
*/
void display(char *filename)
{
	
		
	//打开图片
	int bmp_fd = open(filename,O_RDWR);
		if(bmp_fd < 0)
		{
			perror("打开bmp图片失败！");
			return ;
		}
	
	char head[54]={0};
	
	read(bmp_fd,head,54);
	
	int width = head[18] | head[19] << 8 | head[20] << 16 | head[21]<<24;
	int hight = head[22] | head[23] << 8 | head[24] << 16 | head[25]<<24;
	int x_offset = 400-width/2;
	int y_offset = 240-hight/2;
	
	char bmp_buf[width*hight*3];
	
	//读取图片信息
	int ret = read(bmp_fd,bmp_buf,sizeof(bmp_buf));
		if(ret < 0)
		{
			perror("bmp读取失败！\n");
			close(bmp_fd);
			
			return ;
	
		}
	
	//关闭图片
	close(bmp_fd);
	
	int lcd_buf[width*hight];
	
	int i,x,y;
	
	//三个char 合成一个 int
	for(i=0; i<width*hight; i++)
	{
		lcd_buf[i]=bmp_buf[i*3] | bmp_buf[i*3+1]<<8 | bmp_buf[i*3+2]<<16;
	}
	
	for(y=0; y<hight; y++)
	{
		for(x=0; x<width; x++)
		{
			//每次复制一个像素点
			if(width+x_offset<=800 && hight+y_offset <=480)
			{
				memcpy((devp->map+(x+x_offset)+800*(y+y_offset)), &lcd_buf[x+(hight-1-y)*width], 4);
			} 
		}
	}
	
}

