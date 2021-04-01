# Redis-C-API
Redis's C API.Linux can use this API to operate the redis database.

## 一、hiredis安装
下载hiredis包或使用redis包中的hiredis。解压后make编译，然后make install安装。<br>
在对使用hiredis的程序进行编译时，gcc要加上-L/usr/local/lib/ -lhiredis。<br>
## 二、hiredis API接口
```c++
redisContext *redisConnect (const char *ip, int port);         //连接redis
void *redisCommand (redisContext *c, const char *format, ...); //向redis 服务发送命令
void freeReplyObject (void *reply);                            //释放执行命令后返回结果reply 占用的内存
void redisFree (redisContext *c);                              //释放与整个redis的连接
```
### 1、连接redis
函数：
```c++
redisContext redisConnect (const char *ip,int port);
```
参数: 很明显传入的是redis的ip 和端口（默认6379）。<br>
返回值: 内置结构体redisContext（主要需要了解的字段就是带注释的err 和errstr）<br>

结构体 redisContext
```c++
	typedef struct redisContext {
		int err; /*错误标志，正确连接标志为0，出错时设置为非零常量*/    
		char errstr[128]; /*存放错误信息的字符串*/
		int fd;
		int flags;
		char *obuf; /* Write buffer */
		redisReader *reader; /* Protocol reader */
	
		enum redisConnectionType connection_type;
		struct timeval *timeout;
	
		struct {
			char *host;
			char *source_addr;
			int port;
		} tcp;
	
		struct {
			char *path;
		} unix_sock;
	} redisContext;
```
### 2、向redis 服务发送命令
函数：
```c++
void *redisCommand(redisContext c,const char *format,…) 
```
参数：<br>
>redisContext c：调用连接接口返回的结果；<br>
>const char* format：传入需要执行的Redis指令。<br>

返回值： 成功，返回结构体指针redisReply* ；失败，返回NULL。<br>

因为函数声明返回值是一个void* ，实际的返回值是结构体指针redisReply* 。<br>
```c++
结构体 redisReply
	typedef struct redisReply {
		int type; //返回的数据类型
		long long integer; //当type是数字类型的时，这个值就是数字的值
		size_t len; //当type是字符串类型时，这个值代表字符串的长度
		char *str; //当出现错误的时候这个值是错误描述；当返回类型是字符串时，这个值是字符串的值
		
		size_t elements; //返回值是数组类型
		struct redisReply **element; //当返回值是数组时，存储返回的数组
	} redisReply;
```

### 3、释放执行命令后返回结果reply 占用的内存
函数：
```c++
*void freeReplyObject(void reply);
```
参数：发送命令接口返回的结果<br>
返回值：void<br>

### 4、释放与整个redis的连接
函数：
```c++
*void redisFree(redisContext c);
```
参数：连接接口返回的连接结构对象。<br>

返回值：void。<br>
