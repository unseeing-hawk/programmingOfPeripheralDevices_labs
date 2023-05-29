#include <omp.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>

void func() 
{
  for (int i = 0; i < 500000; i++)
    rand();
}


int main() 
{
  omp_set_num_threads(4);

  std::cout << "My OMP:\n";

  #pragma omp single
  {
    for (int i = 0; i < 20; i++) 
    {
      {
        if (i == 0)
        {
            std::cout << "\nNum of threads: " << omp_get_num_threads() << "\n";
        }
        std::cout << "*";
        func();

      }
    }
  }

  #pragma omp parallel for
  for (int i = 0; i < 20; i++) 
  {
      if (i == 0)
      {
          std::cout << "\n\nNum of threads: " << omp_get_num_threads() << "\n";
      }

      std::cout << 1;
      func();
      std::cout << 2;
      func();
      std::cout << 3;
      func();

    /*#pragma omp critical
    {
      std::cout << 1;
      func();
      std::cout << 2;
      func();
      std::cout << 3;
      func();
    }*/
  }

  std::cout << "\nMy Locks:\n";
  omp_lock_t lock;
  omp_init_lock(&lock);

  #pragma omp parallel
  {
    #pragma omp sections nowait
    {
      #pragma omp section
      {
        omp_set_lock(&lock);

        for (int i = 0; i < 20; i++) 
        {
            if (i == 10)
            {
                omp_unset_lock(&lock);
            }
          std::cout << 6;
          func();
        }
      }

      #pragma omp section
      {
          omp_set_lock(&lock);
          for (int i = 0; i < 20; i++)
          {
              std::cout << 7;
              func();
          }
          omp_unset_lock(&lock);
      }
    }

  }
  omp_destroy_lock(&lock);
  system("pause");
}
