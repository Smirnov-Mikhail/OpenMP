// Task2.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <omp.h>

#define ArraySize 100000
using namespace std;
	
int main()
{
	int A[ArraySize];
	int B[ArraySize];
	for (int i = 0; i < ArraySize; ++i)
	{
		A[i] = i % 3;
		B[i] = i % 3;
	}

	double t1, t2, dt;
	t1 = omp_get_wtime();

	int result = 0;
	#pragma omp parallel for reduction (+:result)
	for (int i = 0; i < ArraySize; ++i)
		result += A[i] * B[i];

	t2 = omp_get_wtime();
	dt = t2 - t1;
	cout << "Work time: " << dt << endl;

	cout << "result: " << result << endl;

	return 0;
}

