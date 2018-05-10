// Task2.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <omp.h>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

#define ArraySize 999
const int N = 100000;

using namespace std;

void AddStrToIntVector(int numberLine, string str, vector<vector<int>> &array2N);
void PrintArray2N(vector<vector<int>> &array2N);
//omp_get_thread_num()
int main()
{
	vector<vector<int>> array2N(N, vector<int>(ArraySize, -1));

	int A[ArraySize];
	int B[ArraySize];
	for (int i = 0; i < ArraySize; ++i)
	{
		A[i] = 1;
		B[i] = i % 2;
	}

	/*ofstream f;
	f.open("input.txt", ios::out);
	for (int j = 0; j < N; j++)
	{
		for (int i = 0; i < ArraySize; ++i)
		{
			f << B[i];
		}
		f << "\n";
	}
	f.close();//*/


	double t1, t2, dt;
	t1 = omp_get_wtime();

	int result = 0;
	int currentRead = -1;
	ifstream file("input.txt");

	
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			string str;
			while (getline(file, str)) {
				AddStrToIntVector(currentRead + 1, str, array2N);
				currentRead++;
			}
		}
		#pragma omp section
		{
			int current = 0;
			while (current < N)
			{
				if (currentRead >= current)
				{
					for (int i = 0; i < ArraySize; ++i)
						result += A[i] * array2N[current][i];
					current++;
				}
			}
		}
		/*#pragma omp section
		{
			int current = 0;
			while (current < N)
			{
				if (currentRead >= current)
				{
					int temp = 0;
					for (int i = 0; i < ArraySize; ++i)
					{
						temp += A[i] * array2N[current][i];
					}
					#pragma omp atomic
					result += temp;
					current+=2;
				}
			}
		}
		#pragma omp section
		{
			int current = 1;
			while (current < N)
			{
				if (currentRead >= current)
				{
					int temp = 0;
					for (int i = 0; i < ArraySize; ++i)
					{
						temp += A[i] * array2N[current][i];
					}
					#pragma omp atomic
					result += temp;
					current += 2;
				}
			}
		}*/
	}//*/
	
	// seq
	/*string str;
	while (getline(file, str)) {
		AddStrToIntVector(currentRead + 1, str, array2N);
		currentRead++;
	}

	for (int current = 0; current < N; ++current)
	for (int i = 0; i < ArraySize; ++i)
		result += A[i] * array2N[current][i];//*/

	t2 = omp_get_wtime();
	dt = t2 - t1;
	cout << "Work time: " << dt << endl;

	cout << "result: " << result << endl;

	//PrintArray2N(array2N);
	file.close();
	return 0;
}

void AddStrToIntVector(int numberLine, string str, vector<vector<int>> &array2N)
{
	vector<int> input(ArraySize, -1);
	for (int i = 0; i < ArraySize; ++i)
	{
		input[i] = str[i + 1] - '0';
	}
	array2N[numberLine] = input;
}

void PrintArray2N(vector<vector<int>> &array2N)
{
	for (int j = 0; j < N; j++)
	{
		for (int i = 0; i < 10; ++i)
		{
			cout << array2N[j][i] << " ";
		}
		cout << endl;
	}
}

