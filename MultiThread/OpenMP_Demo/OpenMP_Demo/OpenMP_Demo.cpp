// OpenMP_Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <omp.h>
#include <stdio.h>


int _tmain(int argc, _TCHAR* argv[])
{
    int a[5], i;

#pragma omp parallel
    {
        // Perform some computation.
#pragma omp for
        for (i = 0; i < 5; i++)
            a[i] = i * i;

        // Print intermediate results.
#pragma omp master
        for (i = 0; i < 5; i++)
            printf_s("a[%d] = %d\n", i, a[i]);

        // Wait.
#pragma omp barrier

        // Continue with the computation.
#pragma omp for
        for (i = 0; i < 5; i++)
            a[i] += i;

        // Print intermediate results.
#pragma omp master
        for (i = 0; i < 5; i++)
            printf_s("a[%d] = %d\n", i, a[i]);

    }

	return 0;
}

