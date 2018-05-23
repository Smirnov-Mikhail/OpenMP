#include "stdafx.h"
#include <math.h> 
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <time.h>
#include <mpi.h>
#include <assert.h>

using namespace std;

const int N = 100;// 20000000;

void headOnAlgorithm(int argc, char* argv[]);
void reduceAlgorithm(int argc, char* argv[]);
void test(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	//test(argc, argv);
	//reduceAlgorithm(argc, argv);
	//headOnAlgorithm(argc, argv);
	return 0;
}

// Решение задачи "в лоб".
// Массив размером 20000000
// Для 8 процессов время: 0.93
// Для 6 процессов время: 0.945
// Для 4 процессов время: 1.212
// Для 2 процессов время: 2.33
// Время последовательной работы: 4.751 (с выключенным MPI просто цикл и сравнение)
void headOnAlgorithm(int argc, char* argv[])
{
	int  ProcRank, ProcNum, k, i1, i2;
	double TotalMax, ProcMax = 0.0;
	std::vector<int> arrayN(std::vector<int>(N, -1));
	double startwtime = 0.0;
	double endwtime;
	MPI_Status Status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	// Подготовка данных
	if (ProcRank == 0)
		for (int i = 0; i < N; ++i)
			arrayN[i] = i % 3;

	arrayN[5] = 7;
	arrayN[8] = -2;

	if (ProcRank == 0)
	{
		startwtime = MPI_Wtime();
	}

	// Рассылка данных на все процессы
	MPI_Bcast(&arrayN, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	k = N / ProcNum;
	i1 = k *   ProcRank;
	i2 = k * (ProcRank + 1);
	if (ProcRank == ProcNum - 1) i2 = N;
	for (int i = i1; i < i2; i++)
		if (ProcMax < arrayN[i])
			ProcMax = arrayN[i];

	if (ProcRank == 0) {
		TotalMax = ProcMax;
		for (int i = 1; i < ProcNum; i++) {
			MPI_Recv(&ProcMax, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Status);
			if (TotalMax < ProcMax)
				TotalMax = ProcMax;
		}
	}
	else
		MPI_Send(&ProcMax, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

	if (ProcRank == 0)
	{
		cout << "Max = " << TotalMax << endl;
		endwtime = MPI_Wtime();
		cout << (endwtime - startwtime) << endl;
	}
	MPI_Finalize();
}

// Решение задачи с использованием встроенных функций.
// Массив размером 20000000
// Для 8 процессов время: 0.875
// Для 6 процессов время: 0.927
// Для 4 процессов время: 1.2
// Для 2 процессов время: 2.11
// В целом во всех случаях получилось быстрее, где-то совсем незначительно, где-то уже в процентах ощутимо.
void reduceAlgorithm(int argc, char* argv[])
{
	int  ProcRank, ProcNum, k, i1, i2;
	int TotalMax, ProcMax = 0;
	std::vector<int> arrayN(std::vector<int>(N, -1));
	double startwtime = 0.0;
	double endwtime;
	MPI_Status Status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	// Подготовка данных
	if (ProcRank == 0)
		for (int i = 0; i < N; ++i)
			arrayN[i] = i % 3;

	arrayN[5] = 7;
	arrayN[8] = -2;

	if (ProcRank == 0)
	{
		startwtime = MPI_Wtime();
	}

	// Рассылка данных на все процессы
	MPI_Bcast(&arrayN, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	k = N / ProcNum;
	i1 = k *   ProcRank;
	i2 = k * (ProcRank + 1);
	if (ProcRank == ProcNum - 1) i2 = N;
	for (int i = i1; i < i2; i++)
		if (ProcMax < arrayN[i])
			ProcMax = arrayN[i];

	cout << "My max " << ProcMax << " from " << ProcRank << endl;
	MPI_Reduce(&ProcMax, &TotalMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (ProcRank == 0)
	{
		cout << "Max = " << TotalMax << endl;
		endwtime = MPI_Wtime();
		cout << (endwtime - startwtime) << endl;
	}
	MPI_Finalize();
}

// Creates an array of random numbers. Each number has a value from 0 - 1
int *createArray(int num_elements) {
	int *arrayMain = (int *)malloc(sizeof(int) * num_elements);
	assert(arrayMain != NULL);
	for (int i = 0; i < num_elements; i++) {
		arrayMain[i] = 0;// rand() % 40;
	}
	arrayMain[5] = 7;
	arrayMain[1] = 12;
	arrayMain[8] = -2;
	arrayMain[555] = 72;
	return arrayMain;
}

// Решение задачи с использованием встроенных функций.
// Массив размером 20000000
// Для 8 процессов время: 0.0489
// Для 6 процессов время: 0.0496
// Для 4 процессов время: 0.0503
// Для 2 процессов время: 0.0447
void test(int argc, char* argv[])
{
	int ProcRank, ProcNum, k, ProcMax = 0, TotalMax = 0;
	double startwtime = 0.0;
	double endwtime;

	srand(time(NULL));

	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	k = N / ProcNum;

	int *arrayMain = NULL;
	if (ProcRank == 0) 
	{
		arrayMain = createArray(k * ProcNum);
	}

	if (ProcRank == 0)
	{
		startwtime = MPI_Wtime();
	}

	int *arrayProc = (int *)malloc(sizeof(int) * k);
	assert(arrayProc != NULL);

	MPI_Scatter(arrayMain, k, MPI_INT, arrayProc, k, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < k - 1; i++)
	{
		if (ProcMax < arrayProc[i])
			ProcMax = arrayProc[i];
	}

	MPI_Reduce(&ProcMax, &TotalMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

	if (ProcRank == 0) {
		cout << "TotalMax: " << TotalMax << endl;
		endwtime = MPI_Wtime();
		cout << (endwtime - startwtime) << endl;
		free(arrayMain);
	}

	free(arrayProc);

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
}