// Task4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>
#include <omp.h>

#define ArraySize 100
using namespace std;

//#pragma omp parallel num_threads(3)


int main()
{
	//#pragma omp parallel 
	//cout << "Hello world" << endl;

	int A[ArraySize][ArraySize];
	for (int i = 0; i < ArraySize; ++i)
		for (int j = 0; j < ArraySize; ++j)
			A[i][j] = i;
	A[7][0] = 22;
	A[3][2] = ArraySize + 172;
	A[5][3] = -3;

	/*for (int i = 0; i < ArraySize; ++i)
	{
		for (int j = 0; j < ArraySize; ++j)
			cout << A[i][j] << " ";
		cout << endl;
	}*/

	int shared_min_array[ArraySize];
	for (int i = 0; i < ArraySize; ++i)
		shared_min_array[i] = A[i][0];

	int shared_max = A[0][0];

	double t1, t2, dt;
	t1 = omp_get_wtime();

	#pragma omp parallel
	{
		int min[ArraySize];
		#pragma omp for
		for (int i = 0; i < ArraySize; ++i)
		{
			min[i] = A[i][0];
		}

		#pragma omp for
		for (int i = 0; i < ArraySize; ++i)
		{
			for (int j = 0; j < ArraySize; ++j)
			{
				if (min[i] > A[i][j])
					min[i] = A[i][j];
			}

			#pragma omp critical
			{
				if (shared_min_array[i] > min[i])
					shared_min_array[i] = min[i];
			}
		}

		int max = shared_min_array[0];
		#pragma omp for nowait
		for (int i = 0; i < ArraySize; ++i)
		{
			if (max < min[i])
				max = min[i];
		}

		#pragma omp critical 
		{
			if (max > shared_max)
				shared_max = max;
		}
	}

	t2 = omp_get_wtime();
	dt = t2 - t1;
	cout << "Work time: " << dt << endl;

	cout << "max: " << shared_max << endl;
	return 0;
}

