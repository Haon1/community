#include "../../header/client.h"


/*
	init_dev: 初始化硬件设备
	@p：设备管理器指针
*/
void init_dev()
{
	//打开lcd
	devp->lcd_fd = open("/dev/fb0",O_RDWR);
	
		if(devp->lcd_fd < 0)
		{
			fprintf(stderr,"%s","init lcd failed: ");
			perror("");
			
			exit(-1);
		}
		
	//开启映射
	devp->map = mmap(NULL, LCD_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, devp->lcd_fd, 0);
		
		if(devp->map == MAP_FAILED)
		{
			fprintf(stderr,"%s","mmap failed: ");
			perror("");
			
			//回收资源
			close(devp->lcd_fd);
			
			exit(-1);
		}
	
	#ifdef ARM
	//初始化字库
	tp = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
	#endif
}
