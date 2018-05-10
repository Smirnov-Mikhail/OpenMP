#include "stdafx.h"
#include <math.h> 
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <time.h> 
// массив по частям, функция для максимума.
using namespace std;

const int N = 16;// 20000000;

void headOnAlgorithm(int argc, char* argv[]);
void reduceAlgorithm(int argc, char* argv[]);
void reduceInPartAlgorithm(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	reduceInPartAlgorithm(argc, argv);
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

void reduceInPartAlgorithm(int argc, char* argv[])
{
	srand(time(NULL));

	int  ProcRank, ProcNum, k;
	int TotalMax = 0, ProcMax = 0;
	
	double startwtime = 0.0;
	double endwtime;
	MPI_Status Status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	k = N / ProcNum;
	std::vector<int> rArrayN(k, -2);
	std::vector<int> arrayN(std::vector<int>(k, -1));
	// Подготовка данных
	if (ProcRank == 0)
	{
		for (int i = 0; i < k; ++i)
		{
			arrayN[i] = rand() % 10;
		}
		arrayN[0] = 17;
		arrayN[1] = -2;
	}

	if (ProcRank == 0)
	{
		startwtime = MPI_Wtime();
	}
	
	// Рассылка данных на все процессы
	MPI_Scatter(&arrayN, k, MPI_INT, &rArrayN, k, MPI_INT, 0, MPI_COMM_WORLD);

	cout << "ProcRank " << ProcRank << endl;
	for (int i = 0; i < k; i++)
	{
		cout << "ProcRank " << ProcRank << " rArrayN["<< i << "]: " << rArrayN[i] << endl;
		if (ProcMax < rArrayN[i])
			ProcMax = rArrayN[i];
	}

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