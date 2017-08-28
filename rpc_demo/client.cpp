#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "rpc_test.h"     // 引用MIDL 生成的头文件

int local_intAdd(int x, int y)
{
    //printf("%d + %d = %d\n", x, y, x + y);
    return x + y;
}

typedef long long Int64;

Int64 Now()
{
    Int64 xNow;
    SYSTEMTIME st;
    ::GetLocalTime(&st);
    ::SystemTimeToFileTime(&st, (LPFILETIME)&xNow);
    return xNow;
}

int main(int argc, char * argv[])
{
    unsigned char * pszStringBinding = NULL;
    //if (argc != 2)
    //{
    //    printf("Usage:%s <Hello Text>/n", argv[0]);
    //    return 1;
    //}

    // 用Named Pipe 作为RPC 的通道。参见server.c 中的RpcServerUseProtseqEp() 部分
    // 第3 个参数NetworkAddr 如果取NULL，那么就是连接本机服务
    // 否则要取////servername 这样的格式，例如你的计算机名为jack，那么就是//jack
    //RpcStringBindingCompose(NULL, (unsigned char*)"ncacn_np", /*(unsigned char*)"////servername"*/ NULL, (unsigned char*)"//pipe//{8dd50205-3108-498f-96e8-dbc4ec074cf9}", NULL, &pszStringBinding);
    RpcStringBindingCompose(NULL, (unsigned char*)"ncacn_ip_tcp", (unsigned char*)"localhost" /*NULL*/, (unsigned char*)"13521", NULL, &pszStringBinding);

    // 绑定接口，这里要和 Hello.acf 的配置一致，那么就是HelloWorld_Binding
    RpcBindingFromStringBinding(pszStringBinding, &HelloWorld_Binding);



    Int64 xBegin = Now();
    for (int i = 0; i < 10000; i++)
    {
        int x = local_intAdd(10, 20);
    }
    Int64 xEnd = Now();

    printf("%lld\n", xEnd - xBegin);

    // 下面是调用服务端的函数了
    xBegin = Now();
    for (int i = 0; i < 10000; i++)
    {
        //RpcTryExcept;
        //{
        int x = intAdd(10, 20);
        //Hello((unsigned char*)argv[0]);
        //Shutdown();
    //}
    //RpcExcept(1)
    //{
    //    printf("RPC Exception %d/n", RpcExceptionCode());
    //}
    //RpcEndExcept;
    }
    xEnd = Now();
    printf("%lld\n", xEnd - xBegin);



    // 释放资源
    RpcStringFree(&pszStringBinding);
    RpcBindingFree(&HelloWorld_Binding);
    system("pause");
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
