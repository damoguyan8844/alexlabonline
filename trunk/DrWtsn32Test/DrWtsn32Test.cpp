// DrWtsn32Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
/********************************************************************/
/* 程序：lesson_2, 结合Dr.Watson与工程map文件定位错误                                  */
/* 功能：多线程环境中利用除0错误使程序产生异常，被Dr.Watson捕获之       */
/* 作者：coding (http://blog.csdn.net/coding_hello)                                                     */
/* 日期：2008-09-29                                                                                                    */
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