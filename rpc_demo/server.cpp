#include <stdlib.h>
#include <stdio.h>
#include "rpc_test.h"     // 引用MIDL 生成的头文件

/**
* 这是我们在IDL 中定义的接口方法
* 需要注意一点，IDL 里面的声明是：void Hello([in,string]const char * psz);
* 但是这里变成了const unsigned char *，为什么呢？
* 参见MSDN 中的MIDL Command-Line Reference -> /char Switch
* 默认的编译选项，对 IDL 中的char 按照unsigned char 处理
*/

int intAdd(int x, int y)
{
    printf("%d + %d = %d\n", x, y, x + y);
    return x + y;
}

void Hello(const unsigned char * psz)
{
    printf("%s/n", psz);
}


/** 这也是我们在IDL 中定义的接口方法，提供关闭server 的机制*/
void Shutdown(void)
{
    // 下面的操作将导致 RpcServerListen() 退出
    RpcMgmtStopServerListening(NULL);
    RpcServerUnregisterIf(NULL, NULL, FALSE);
}

int main(int argc, char * argv[])
{
    //RPC_S_PROTSEQ_NOT_SUPPORTED
    // 用Named Pipe 作为RPC 的通道，这样EndPoint 参数就是Named Pipe 的名字
    // 按照Named Pipe 的命名规范，/pipe/pipename，其中pipename 可以是除了/ 
    // 之外的任意字符，那么这里用一个GUID 串来命名，可以保证不会重复
    //RpcServerUseProtseqEp((unsigned char *)"ncacn_np", 20, (unsigned char *)"//pipe//{4556509F-618A-46CF-AB3D-ED736ED66477}", NULL);
    RpcServerUseProtseqEp((unsigned char *)"ncacn_ip_tcp", RPC_C_PROTSEQ_MAX_REQS_DEFAULT, (unsigned char *)"13521", NULL);

    ///正确的写法应该为\\pipe\\{8dd50205-3108-498f-96e8-dbc4ec074cf9}

    // 注册接口，HelloWorld_v1_0_s_ifspec 是在MIDL 生成的Hello.h 中定义的
    RpcServerRegisterIf(HelloWorld_v1_0_s_ifspec, NULL, NULL);

    // 开始监听，本函数将一直阻塞
    //RpcServerListen(1, 20, FALSE);
    RpcServerListen(1, 20, TRUE);


    return 0;
}

// 下面的函数是为了满足链接需要而写的，没有的话会出现链接错误
void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len)
{
    return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR *ptr)
{
    free(ptr);
}

