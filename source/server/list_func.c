/**
 *	Community - Server - list_func
 */

#include "../../header/server.h"


/*
	init_client:初始化一个用户信息
	@socket	:  客户的socket	
	@addr	：客户的地址信息
*/
struct client *init_client(char *name,int socket,struct sockaddr_in addr)
{
	struct client *new = (struct client *)malloc(sizeof(struct client));
	strcpy(new->name,name);
	new->socket = socket;
	new->addr = addr;
	
	return new;
}

/*
	add_client:新增一个客户到列表中
	@socket	:  客户的socket	
	@addr：客户的地址信息
*/
void add_client(char *name,int socket,struct sockaddr_in addr)
{
	struct client *new_client = init_client(name,socket,addr);
	
	list_add_tail(&new_client->list,head);
}


/*
	del_client:把一个客户从列表中删除
	@socket：要删除的客户的socket
*/
void del_client(int socket)
{
	struct list_head *pos = NULL;
	
	list_for_each(pos,head)
	{
		struct client *p = list_entry(pos,struct client,list);
		
		if(p->socket == socket)
		{
			list_del(pos);
			free(p);
			
			people--;
			
			return ;
		}
	}
}


/*
	show_client:显示所有连接的客户端
*/
void show_client()
{
	system("clear");
	struct list_head *pos = NULL;
	
	list_for_each(pos,head)
	{
		struct client *p = list_entry(pos,struct client,list);
		
		printf("[%d] name:%s socket:%d  ip:%s  port:%d\n",p->code,p->name,p->socket,\
													inet_ntoa(p->addr.sin_addr),\
													ntohs(p->addr.sin_port));
	}
	
	printf("\n\n");
}


/*
	recode:客户排序
*/
void recode()
{
	struct list_head *pos = NULL;
	int code = 1;
	
	list_for_each(pos,head)
	{
		struct client *p = list_entry(pos,struct client,list);
		
		p->code = code++;
	}
}


/*
	get_client:通过code获取客户信息
	@code: 要获取信息客户的code
*/
struct client *get_client_bycode(int code)
{
	struct list_head *pos = NULL;
	
	list_for_each(pos,head)
	{
		struct client *p = list_entry(pos,struct client,list);
		
		if(p->code == code)
		{
			return p;
		}
	}
	
	return NULL;
}

/*
	get_client_byname:通过name获取客户信息
	@name: 要获取信息客户的name
*/
struct client *get_client_byname(char *name)
{
	struct list_head *pos = NULL;
	
	list_for_each(pos,head)
	{
		struct client *p = list_entry(pos,struct client,list);
		
		if(strcmp(p->name,name)==0)
		{
			return p;
		}
	}
	
	return NULL;
}

/*
	get_client_bysock:通过socket获取客户信息
	@socket: 要获取信息客户的code
*/
struct client *get_client_bysock(int socket)
{
	struct list_head *pos = NULL;
	
	list_for_each(pos,head)
	{
		struct client *p = list_entry(pos,struct client,list);
		
		if(p->socket == socket)
		{
			return p;
		}
	}
	
	return NULL;
}

/*
	putmsg:给小区每户人家发送征婚信息
	@msg：征婚消息内容
*/
void putmsg(char *msg)
{
	struct list_head *pos = NULL;
	
	list_for_each(pos,head)
	{
		struct client *p = list_entry(pos,struct client,list);
		
		write(p->socket,msg,strlen(msg));
	}
}

/*
	get_list:获取用户列表到信息消息中
*/
char *get_list()
{
	char *list = (char *)malloc(1000);
	bzero(list,1000);
	
	struct list_head *pos = NULL;
	
	list_for_each(pos,head)
	{
		struct client *p = list_entry(pos,struct client,list);
		
		char info[40]={0};
		
		//按照这个格式订装信息
		sprintf(info,"%d %s %s\n",p->code,p->name,inet_ntoa(p->addr.sin_addr));
		
		//追加到列表中
		strcat(list,info);
	}

	return list;
	
}


