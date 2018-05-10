// Task6.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>
#include <omp.h>

#define ArraySize 200000
using namespace std;

int main()
{
	int A[ArraySize], sum = 0;
	for (int i = 0; i < ArraySize; ++i)
		A[i] = i;

	double t1, t2, dt;

	// Тест для атомика.
	t1 = omp_get_wtime();

	//#pragma omp parallel num_threads(1) 
	#pragma omp parallel for
	for (int i = 0; i < ArraySize; ++i)
	{
		#pragma omp atomic
		sum &= A[i];
	}

	t2 = omp_get_wtime();
	dt = t2 - t1;
	cout << "Work time atomic: " << dt << endl;
	cout << "Sum: " << sum << endl;

	// Тест для критикала.
	t1 = omp_get_wtime();

	#pragma omp parallel for
	for (int i = 0; i < ArraySize; ++i)
	{
		#pragma omp critical
		sum &= A[i];
	}

	t2 = omp_get_wtime();
	dt = t2 - t1;
	cout << "Work time critical: " << dt << endl;
	cout << "Sum: " << sum << endl;

	// Тест для замка.
	t1 = omp_get_wtime();

	omp_lock_t lock;
	omp_init_lock(&lock);
	#pragma omp parallel for
	for (int i = 0; i < ArraySize; ++i)
	{
		omp_set_lock(&lock);
		sum &= A[i];
		omp_unset_lock(&lock);
	}

	t2 = omp_get_wtime();
	dt = t2 - t1;
	cout << "Work time lock: " << dt << endl;
	cout << "Sum: " << sum << endl;

	// Тест для редукции.
	t1 = omp_get_wtime();

	#pragma omp parallel for reduction (&:sum)
	for (int i = 0; i < ArraySize; ++i)
		sum &= A[i];

	t2 = omp_get_wtime();
	dt = t2 - t1;
	cout << "Work time reduction: " << dt << endl;
	cout << "Sum: " << sum << endl;

	return 0;
}

