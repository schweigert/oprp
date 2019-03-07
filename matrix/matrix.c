#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
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

matrix_t *matrix_sort(matrix_t *matrix_a) {
   int i, j;
   double swap;
   int max = matrix_a->rows * matrix_a->cols;
   matrix_t *matrix_r = matrix_create(matrix_a->rows, matrix_a->cols);

   double *vector = matrix_r->data[0];

   for (i = 0; i < max; i++) {
      vector[i] = matrix_a->data[0][i];
      
      for (j = 0; j < i; j++) {
         if (vector[i] < vector[j]) {
            swap = vector[i];
            vector[i] = vector[j];
            vector[j] = swap;
         }
      }
   }

   return matrix_r;
}

void matrix_print(matrix_t *m) {
   int i, j;
   for (i = 0; i < m->rows; i++) {
      for (j = 0; j < m->cols; j++) {
         printf("%.1f ", m->data[i][j]);
      }
      printf("\n");
   }
   fflush(stdout);
}