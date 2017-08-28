#include <stdlib.h>
#include <stdio.h>
#include "rpc_test.h"     // ����MIDL ���ɵ�ͷ�ļ�

/**
* ����������IDL �ж���Ľӿڷ���
* ��Ҫע��һ�㣬IDL ����������ǣ�void Hello([in,string]const char * psz);
* ������������const unsigned char *��Ϊʲô�أ�
* �μ�MSDN �е�MIDL Command-Line Reference -> /char Switch
* Ĭ�ϵı���ѡ��� IDL �е�char ����unsigned char ����
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


/** ��Ҳ��������IDL �ж���Ľӿڷ������ṩ�ر�server �Ļ���*/
void Shutdown(void)
{
    // ����Ĳ��������� RpcServerListen() �˳�
    RpcMgmtStopServerListening(NULL);
    RpcServerUnregisterIf(NULL, NULL, FALSE);
}

int main(int argc, char * argv[])
{
    //RPC_S_PROTSEQ_NOT_SUPPORTED
    // ��Named Pipe ��ΪRPC ��ͨ��������EndPoint ��������Named Pipe ������
    // ����Named Pipe �������淶��/pipe/pipename������pipename �����ǳ���/ 
    // ֮��������ַ�����ô������һ��GUID �������������Ա�֤�����ظ�
    //RpcServerUseProtseqEp((unsigned char *)"ncacn_np", 20, (unsigned char *)"//pipe//{4556509F-618A-46CF-AB3D-ED736ED66477}", NULL);
    RpcServerUseProtseqEp((unsigned char *)"ncacn_ip_tcp", RPC_C_PROTSEQ_MAX_REQS_DEFAULT, (unsigned char *)"13521", NULL);

    ///��ȷ��д��Ӧ��Ϊ\\pipe\\{8dd50205-3108-498f-96e8-dbc4ec074cf9}

    // ע��ӿڣ�HelloWorld_v1_0_s_ifspec ����MIDL ���ɵ�Hello.h �ж����
    RpcServerRegisterIf(HelloWorld_v1_0_s_ifspec, NULL, NULL);

    // ��ʼ��������������һֱ����
    //RpcServerListen(1, 20, FALSE);
    RpcServerListen(1, 20, TRUE);


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

