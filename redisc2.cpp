#include<stdio.h>
#include<string.h>
#include<hiredis/hiredis.h>

int main()
{
	//char ipaddr[] = "172.27.0.17";
	char ipaddr[] = "127.0.0.1";
	char *ip = ipaddr;
	int port = 6379;
	/* 连接redis */
	redisContext *c = redisConnect(ip, port);
	if(c->err)
	{
		redisFree(c);
		printf("connect to redis failed!\n");
		return 0; 
	}
	printf("connect to redis success!\n");		

	/* SET */
	/* 需要向redis发出的命令 */
	char command[1024];	
	char commandstr[1024];
	char *rediscommand;
	char *rediscommandstr;

	rediscommand = command;
	rediscommandstr = commandstr;

	redisReply *reply;
	
	while(1)
	{	
		memset(rediscommand,0,sizeof(command));
		memset(rediscommandstr,0,sizeof(commandstr));
		
		printf("input command:");
		fgets(rediscommandstr,sizeof(commandstr),stdin);
		strncpy(rediscommand,rediscommandstr,strlen(commandstr)-1);
		
		if(strcmp(rediscommand,"end") == 0)
		{  
			printf("disconnect from redis, exit.\n");
			break;	
		}
		/* 向redis发送命令 */
		reply = (redisReply *)redisCommand(c, rediscommand); // 命令发送成功返回一个redisreply结构体
	
		/* 命令发送失败 */
		if(reply == NULL) // 返回值为NULL则说明发送命令失败
		{
			printf("command send failure\n");
			break;
		}
			
		/* 命令执行失败 */
		switch(reply->type)
		{
			case REDIS_ERR: printf("ERR!\n");break;
			case REDIS_ERR_IO: printf("Error in read or write!\n");break;
                        case REDIS_ERR_PROTOCOL: printf("Protocol error!\n");break;
                        case REDIS_ERR_EOF: printf("End of file!\n");break;
                        case REDIS_ERR_OOM: printf("Out of memory\n");break;
                        case REDIS_ERR_OTHER: printf("Everything else...!\n");break;
			default:break; 
		}
	
		/* 命令执行完成后释放reply占用的内存 */
		freeReplyObject(reply);		
		printf("%s execute success\n", rediscommand);
        
        }
	
        /* 命令执行完成后释放reply占用的内存 */
	redisFree(c);	
	return 0;
}

