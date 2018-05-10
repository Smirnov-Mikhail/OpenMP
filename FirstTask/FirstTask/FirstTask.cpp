// FirstTask.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>
#include <omp.h>

#define ArraySize 100000
using namespace std;

int main()
{
	const int N = 2;
	const int M = 1000000;
	std::vector<std::vector<int>> array2N(N, std::vector<int>(M, -1));

	cout << array2N[0][0] << endl;
	array2N[1][227] = 3;
	int shared_max_array = array2N[0][0];
	for (int count = 1; count <= 8; count++)
	{
		double t1, t2, dt;
		t1 = omp_get_wtime();
		omp_set_num_threads(count);
		#pragma omp parallel
		{
			int max = array2N[0][0];
			#pragma omp for
			for (int i = 0; i < N; ++i)
				for (int j = 0; j < M; ++j)
				{
					if (max < array2N[i][j])
						max = array2N[i][j];
				}

			#pragma omp critical 
			{
				if (max > shared_max_array)
					shared_max_array = max;
			}
		}

		t2 = omp_get_wtime();
		dt = t2 - t1;
		cout << "max: " << shared_max_array << endl;
		cout << "Work time " << count << " threads: " << dt << endl;
	}
	
	return 0;
}

/*// FirstTask.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include <algorithm>
#include <omp.h>

#define ArraySize 100000
using namespace std;

//#pragma omp parallel num_threads(3)


int main()
{
	std::vector<std::vector<int>> distanceArray(100, std::vector<int>(9000000, -1)); // ~3GB

	cout << distanceArray[0][0] << endl;
	int A[ArraySize];
	for (int i = 0; i < ArraySize; ++i)
		A[i] = i;
	A[7] = 22;
	A[322] = ArraySize + 172;
	A[556] = -3;
	int shared_max_array = A[0];

	double t1, t2, dt;
	t1 = omp_get_wtime();

	#pragma omp parallel
	{
		int max = A[0];
		#pragma omp for nowait
		for (int i = 0; i < ArraySize; ++i)
		{
			if (max < A[i])
				max = A[i];
		}

		#pragma omp critical 
		{
			if (max > shared_max_array)
				shared_max_array = max;
		}
	}

	t2 = omp_get_wtime();
	dt = t2 - t1;
	cout << "Work time: " << dt << endl;

	cout << "max: " << shared_max_array << endl;
	return 0;
	}*/