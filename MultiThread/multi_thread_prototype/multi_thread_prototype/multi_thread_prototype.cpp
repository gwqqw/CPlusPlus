// multi_thread_prototype.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
using namespace std;

HANDLE ghMutex; 
HANDLE hThread[2];

//DWORD WINAPI Thread1(LPVOID lpParm)
//{
//    //DWORD dwCount=0, dwWaitResult; 
//    for (int i = 0; i < 100; ++i)
//    {
//        //dwWaitResult = WaitForSingleObject( 
//        //    ghMutex,    // handle to mutex
//        //    INFINITE);  // no time-out interval
//
//        //switch (dwWaitResult) 
//        //{
//            // The thread got ownership of the mutex
//        //case WAIT_OBJECT_0: 
//            cout << "thread 1 is " << i << endl;
//        //case WAIT_ABANDONED: 
//        //    return FALSE;
//        //}
//
//    }
//    return 0;
//}

DWORD WINAPI Thread1(LPVOID lpParm)
{
    for (int i = 0; i < 100; ++i)
    {
        DWORD dStatus = WaitForSingleObject(ghMutex, INFINITE);
        if (WAIT_ABANDONED == dStatus)
        {
            cout << "Thread 1: thread 1 is " << " wait abandoned" << endl;
        }
        else if(WAIT_OBJECT_0 == dStatus)
        {
            cout << "Thread 1  is " << i << endl;
            ReleaseMutex(ghMutex);
        }
        else if(WAIT_TIMEOUT == dStatus)
        {
            cout << "Thread 1 is " << " wait timeout" << endl;
        }
    }
    return TRUE;
}

DWORD WINAPI Thread2(LPVOID lpParm)
{
    for (int i = 0; i < 100; ++i)
    {
        DWORD dStatus = WaitForSingleObject(ghMutex, INFINITE);
        if (WAIT_ABANDONED == dStatus)
        {
            cout << "Thread 2 is " << " wait abandoned" << endl;
        }
        else if(WAIT_OBJECT_0 == dStatus)
        {
            //cout << "thread 1 is" << " wait object 0" << endl;
            cout << "Thread 2 is " << i << endl;
            ReleaseMutex(ghMutex);
        }
        else if(WAIT_TIMEOUT == dStatus)
        {
            cout << "Thread 2 is " << " wait timeout" << endl;
            //cout << "thread 2 is" << i << endl;
        }
    }
    return TRUE;
}


int _tmain(int argc, _TCHAR* argv[])
{
    DWORD dwThreadIDs[2];

    ghMutex = CreateMutex( 
        NULL,              // default security attributes
        FALSE,             // initially not owned
        NULL);      

    hThread[0] = CreateThread(NULL, 0, Thread1, NULL, 0, &dwThreadIDs[0]);
    hThread[1] = CreateThread(NULL, 0, Thread2, NULL, 0, &dwThreadIDs[1]);
    //WaitForSingleObject(hThread[1],INFINITE);
    //WaitForSingleObject(hThread[0],INFINITE);

    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
    cout << "all end " << endl;

	return 0;
}

