#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <locale.h>
#include "matrix.h"

matrix_t* matrix_one;
matrix_t* matrix_two;

double wtime() {
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}

void doSomething() {
   matrix_t* matrix_r = matrix_sum(matrix_one, matrix_two);
}

int main(int argc, char **argv) {
   double start_time, end_time;
    int nrows, ncols;

   if ((argc != 3)) {
      printf("Uso: %s <rows> <cols>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   nrows = atoi(argv[1]); 
   ncols = atoi(argv[2]); 

   matrix_one = matrix_create(nrows, ncols);
   matrix_two = matrix_create(nrows, ncols);

   matrix_randfill(matrix_one);
   matrix_randfill(matrix_two);

   start_time = wtime();
   doSomething();
   end_time = wtime();

   matrix_destroy(matrix_one);
   matrix_destroy(matrix_two);

   setlocale(LC_NUMERIC, "");
   printf("%'.8f\n", end_time - start_time);
   fflush(stdout);

   return EXIT_SUCCESS;
}
