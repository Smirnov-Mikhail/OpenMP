// FirstTask.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <omp.h>
#include <cmath>

#define ArraySize 10000000
using namespace std;

//#pragma omp parallel num_threads(3)

double F(double x)
{
	return x * x;
}

int main()
{
	double A = -15;
	double B = 15;
	double h = (B - A) / ArraySize;

	double t1, t2, dt;
	t1 = omp_get_wtime();

	int sum_array = 0;
	//omp_set_num_threads(1);
	#pragma omp parallel for reduction(+:sum_array) // вот тут выигрываем по времени, в сравнении с одним потоком.
	for (int i = 0; i < ArraySize; i++)
			sum_array += F(A + i * h);

	cout << "Result: " << h * sum_array << endl;


	t2 = omp_get_wtime();
	dt = t2 - t1;
	cout << "Work time: " << dt << endl;
	return 0;
}

