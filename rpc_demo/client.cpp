#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "rpc_test.h"     // ����MIDL ���ɵ�ͷ�ļ�

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

    // ��Named Pipe ��ΪRPC ��ͨ�����μ�server.c �е�RpcServerUseProtseqEp() ����
    // ��3 ������NetworkAddr ���ȡNULL����ô�������ӱ�������
    // ����Ҫȡ////servername �����ĸ�ʽ��������ļ������Ϊjack����ô����//jack
    //RpcStringBindingCompose(NULL, (unsigned char*)"ncacn_np", /*(unsigned char*)"////servername"*/ NULL, (unsigned char*)"//pipe//{8dd50205-3108-498f-96e8-dbc4ec074cf9}", NULL, &pszStringBinding);
    RpcStringBindingCompose(NULL, (unsigned char*)"ncacn_ip_tcp", (unsigned char*)"localhost" /*NULL*/, (unsigned char*)"13521", NULL, &pszStringBinding);

    // �󶨽ӿڣ�����Ҫ�� Hello.acf ������һ�£���ô����HelloWorld_Binding
    RpcBindingFromStringBinding(pszStringBinding, &HelloWorld_Binding);



    Int64 xBegin = Now();
    for (int i = 0; i < 10000; i++)
    {
        int x = local_intAdd(10, 20);
    }
    Int64 xEnd = Now();

    printf("%lld\n", xEnd - xBegin);

    // �����ǵ��÷���˵ĺ�����
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



    // �ͷ���Դ
    RpcStringFree(&pszStringBinding);
    RpcBindingFree(&HelloWorld_Binding);
    system("pause");
    return 0;
}


// ����ĺ�����Ϊ������������Ҫ��д�ģ�û�еĻ���������Ӵ���
void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len)
{
    return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR *ptr)
{
    free(ptr);
}
