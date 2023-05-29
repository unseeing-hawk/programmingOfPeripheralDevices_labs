#include <omp.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>

void func()
{
	for(int i= 0; i < 500000; i++)
		std::rand();
}

int main()
{
	omp_set_num_threads(4);

#pragma omp parallel for
	for (int i = 0; i < 100; i++)
	{
		std::cout << i << " ";
		func();
	}

	std::cout << "\n\n";

#pragma omp parallel
	{
#pragma omp sections nowait
		{
#pragma omp section
			for (int i = 0; i < 10; i++)
			{
				std::cout << 1;
				func();
			}
#pragma omp section
			for (int i = 0; i < 20; i++)
			{
				std::cout << 2;
				func();
			}
		}
#pragma omp barrier
		for (int i = 0; i < 10; i++)
		{
			std::cout << 3;
			func();
		}
	}

	std::cout << "\n\nParallel sections directive:" << std::endl;
	int time1 = GetTickCount();
	omp_set_num_threads(2);

	for (int i = 0; i < 2; i++)
	{
#pragma omp parallel sections
		{

#pragma omp section
			for (int j = 0; j < 100; j++)
			{
				std::cout << 1;
				func();
			}

#pragma omp section
			for (int j = 100; j < 200; j++)
			{
				std::cout << 2;
				func();
			}
		}
	}
	int time1end = GetTickCount();
	std::cout << "\nTIME: " << time1end - time1 << std::endl << std::endl;


	std::cout << "parallel for num_threads(2) directive:" << std::endl;
	int time2 = GetTickCount();

#pragma omp parallel for num_threads(2)
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 200; j++)
		{
			std::cout << 1;
			func();
		}
	}
	int time2end = GetTickCount();
	std::cout << "\nTIME: " << time2end - time2 << std::endl << std::endl;

	std::cout << "Without parallel directives: " << std::endl;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 200; j++)
		{
			std::cout << 1;
			func();
		}
	}
	int time3end = GetTickCount();
	std::cout << "\nTIME: " << time3end - time2end << std::endl;

	system("pause");
	return 0;
}
