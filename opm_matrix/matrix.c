#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <omp.h>

#include "matrix.h"

matrix_t *matrix_create(int rows, int cols) {
   matrix_t *ret = NULL;
   double *raw_data = NULL;
   double **data = NULL;
   int r;

   raw_data = (double *) malloc(sizeof(double) * rows * cols);
   data = (double **) malloc(sizeof(double*) * rows);

   for (r = 0; r < rows; r++) {
      data[r] = raw_data + r * cols;
   }

   ret = (matrix_t*) malloc(sizeof(matrix_t));
   ret->rows = rows;
   ret->cols = cols;

   ret->data = data;

   return ret;
}

void matrix_destroy(matrix_t *m) {
   free((void*)(m->data[0]));
   free((void*)(m->data));
   free((void*)(m));
   return;
}

void matrix_randfill(matrix_t *m) {
   int r, c;
   for (r = 0; r < m->rows; r++) {
      for (c = 0; c < m->cols; c++) {
         m->data[r][c] = random();
      }
   }
}

void matrix_fill(matrix_t *m, double val) {
   int r, c;
   for (r = 0; r < m->rows; r++) {
      for (c = 0; c < m->cols; c++) {
         m->data[r][c] = val;
      }
   }
}

matrix_t *matrix_sum(matrix_t *matrix_a, matrix_t *matrix_b) {
   int r, c;
   matrix_t* matrix_r = matrix_create(matrix_a->rows, matrix_a->cols);

   #pragma omp parallel for schedule(static)
   for (r = 0; r < matrix_a->rows; r++) {
      for (c = 0; c < matrix_a->cols; c++) {
         matrix_r->data[r][c] = matrix_a->data[r][c] + matrix_b->data[r][c];
      }
   }

   return matrix_r;
}

matrix_t *matrix_multiply(matrix_t *matrix_a, matrix_t *matrix_b) {
   int r, c, i;
   matrix_t* matrix_r = matrix_create(matrix_a->rows, matrix_b->cols);

   #pragma omp parallel for schedule(static)
   for(r = 0; r < matrix_r->rows; r++) {
      for(c = 0; c < matrix_r->cols; c++) {
         matrix_r->data[r][c] = 0;
         for (i = 0; i < matrix_a->cols; i++) {
            matrix_r->data[r][c] += matrix_a->data[r][i] * matrix_b->data[i][c];
         }
      }
   }

   return matrix_r;
}

void swap(double *a, double *b) {
    double aux = *a;
    *a = *b;
    *b = aux;
}

int partition(double *vector, int low, int high) {
   double pivot = vector[high];
   int i = (low - 1);

   for (int j = low; j <= high - 1; j++) {
      if (vector[j] <= pivot) {
         i++;

         swap(&vector[i], &vector[j]);
      }
   }
   swap(&vector[i + 1], &vector[high]);
   return (i + 1);
}

void quick_sort(double *vector, int low, int high, int deep) {
   if (low < high)
   {
      int part = partition(vector, low, high);

      if (deep * 2 > omp_get_num_threads()) {
         quick_sort(vector, low, part - 1, deep + 1);
         quick_sort(vector, part + 1, high, deep + 1);
         return;
      }


      #pragma omp parallel
      {
         #pragma omp sections
         {
            #pragma omp section
            {
               quick_sort(vector, low, part - 1, deep++);
            }

            #pragma omp section
            {
               quick_sort(vector, part + 1, high, deep++);
            }
         }
      }
   }  
}

matrix_t *matrix_sort(matrix_t *matrix) {
   int rows_final = matrix->rows;
   int cols_final = matrix->cols;

   matrix_t *resultado = matrix_create(rows_final, cols_final);

   memcpy(resultado->data[0], matrix->data[0], cols_final * rows_final * sizeof(double));
   quick_sort(resultado->data[0], 0, rows_final * cols_final - 1, 0);

   return resultado;
}

void matrix_print(matrix_t *m) {
   int i, j;
   printf("\n--\n");
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         printf("%.1f ", m->data[i][j]);
      }
      printf("\n");
   }
   printf("\n--\n");
   fflush(stdout);
}