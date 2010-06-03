// DrWtsn32Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
/********************************************************************/
/* ����lesson_2, ���Dr.Watson�빤��map�ļ���λ����                                  */
/* ���ܣ����̻߳��������ó�0����ʹ��������쳣����Dr.Watson����֮       */
/* ���ߣ�coding (http://blog.csdn.net/coding_hello)                                                     */
/* ���ڣ�2008-09-29                                                                                                    */
/********************************************************************/
#include <stdio.h>
#include <process.h>
#include <windows.h>
const int cnThreadnum = 4;
UINT WINAPI Worker(LPVOID lpParam)
{
    srand((DWORD)lpParam);
    DWORD dwTid = GetCurrentThreadId();
    int k=100;
    while(k--)
    {
        printf("tid[%u] tmp = %d\n", dwTid, RAND_MAX/(rand()%cnThreadnum));
        Sleep(10);
    }
    return 0;
}
int main(int argc, char* argv[])
{
    HANDLE hThd[cnThreadnum] = {0};
    for(DWORD i=0; i<cnThreadnum; i++)
    {
        hThd[i] = (HANDLE)_beginthreadex(NULL, 0, Worker, (LPVOID)i, 0, NULL);
    }
    printf("Wait...\n");
    WaitForMultipleObjects(cnThreadnum, hThd, TRUE, INFINITE);
    for(int k=0; k<cnThreadnum; k++)
    {
        CloseHandle(hThd[k]);
    }
    printf("Finish!\n");
    system("pause");
    return 0;
}