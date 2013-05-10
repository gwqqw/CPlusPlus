// UsingThreadLocalStorage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

#define  THREADCOUNT 4
DWORD dwTlsIndex;

void ErrorExit(LPSTR lpszMessage)
{
    fprintf(stderr, "%s\n", lpszMessage);
    ExitProcess(0);
}

void CommonFunc(void)
{
    LPVOID lpvData;

    // Retrieve a data pointer for the current thread
    lpvData = TlsGetValue(dwTlsIndex);
    if ( (0 == lpvData) && (GetLastError() != ERROR_SUCCESS))
    {
        ErrorExit("TlsGetValue error");
    }

    // Use the data stored for the current thread.
    printf("common: thread %d: lpvData = %lx\n",
        GetCurrentThread(), lpvData);

    Sleep(5000);
}


DWORD WINAPI ThreadFunc(LPVOID lpParm)
{
    LPVOID lpvData;

    // Initialize the TLS index for this thread.
    //lpvData = (LPVOID) LocalAlloc(LPTR, 256);
    lpvData = (LPVOID) HeapAlloc(GetProcessHeap(), LPTR, 256);
    if (!TlsSetValue(dwTlsIndex, lpvData))
    {
        ErrorExit("TlsSetValue error");
    }

    int * pInt = (int*) lpParm;
    printf("index %d, thread %d : lpvData=%lx\n", *pInt, GetCurrentThread(), lpvData);

    CommonFunc();

    // Release the dynamic memory before the thread returns.
    lpvData = TlsGetValue(dwTlsIndex);
    if (lpvData != 0)
    {
        //LocalFree((HLOCAL) lpvData);
        HeapFree(GetProcessHeap(),LPTR, lpvData);
    }

    return 0;
}



int _tmain(int argc, _TCHAR* argv[])
{
    DWORD IDThread;
    HANDLE hThread[THREADCOUNT];

    // Allocate a TLS index.
    if ((dwTlsIndex = TlsAlloc()) == TLS_OUT_OF_INDEXES)
    {
        ErrorExit("TlsAlloc failed");
    }

    // Create multiple threads
    int j[THREADCOUNT]; 

    for (int i = 0; i < THREADCOUNT; ++i)
    {
        j[i] = i;
        hThread[i] = CreateThread( NULL,  // default security attributes,
                                   0,   // use default stack size
                                   (LPTHREAD_START_ROUTINE)(ThreadFunc), // thread function
                                   (LPVOID) (&j[i]), // no thread function argument
                                   0,   // use default creation flangs
                                   &IDThread);   // retures thread identifier


        // Check the return value  for success
        if (NULL ==hThread[i])
        {
            ErrorExit("CreateThread error\n");
        }
    }

    for(int i = 0; i < THREADCOUNT; ++i)
        WaitForSingleObject(hThread[i], INFINITE);

    TlsFree(dwTlsIndex);
	return 0;
}

