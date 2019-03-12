#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include "matrix.h"

double wtime() {
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec + t.tv_usec / 1000000.0;
}

void doSomething(int nrows, int ncols) {
   matrix_t* matrix_one = matrix_create(nrows, ncols);
   matrix_randfill(matrix_one);

   matrix_t* matrix_two = matrix_create(nrows, ncols);
   matrix_randfill(matrix_two);

   matrix_t* matrix_three = matrix_multiply(matrix_one, matrix_two);
   matrix_t *matrix_four = matrix_sort(matrix_three);

   matrix_destroy(matrix_one);
   matrix_destroy(matrix_two);
   matrix_destroy(matrix_three);
   matrix_destroy(matrix_four);
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

   start_time = wtime();
   
   doSomething(nrows, ncols);

   end_time = wtime();

   printf("%d %d %f\n", nrows, ncols, end_time - start_time);
   fflush(stdout);

   return EXIT_SUCCESS;
}
