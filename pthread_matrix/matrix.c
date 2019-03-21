#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include "matrix.h"

#define N_CPU 4

typedef struct {
	int id;
   matrix_t *matrix_one;
   matrix_t *matrix_two;
   matrix_t *matrix_ret;
} add_thread;

typedef struct {
	int id;
   matrix_t *matrix_one;
   matrix_t *matrix_two;
   matrix_t *matrix_ret;
} mpp_thread;

typedef struct {
	double *vector;
   int low;
   int high;
   int layer;
} sort_thread;

void deploy_quick_sort(double *vector, int low, int high, int layer);

void *add_worker(void *arg) {
   add_thread *addt = (add_thread *) arg;

   int r, c;

   for(r = 0; r < addt->matrix_one->rows; r++) {
      if (r % N_CPU != addt->id) {
         continue;
      }
      for (c = 0; c < addt->matrix_one->cols; c++) {
         addt->matrix_ret->data[r][c] = addt->matrix_one->data[r][c] + addt->matrix_two->data[r][c];
      }
   }

   return NULL;
}

void *multiply_worker(void *arg) {
   mpp_thread *mppt = (mpp_thread *) arg;

   int r, c, i;

   for(r = 0; r < mppt->matrix_ret->rows; r++) {
         if (r % N_CPU != mppt->id) {
            continue;
         }
      for(c = 0; c < mppt->matrix_ret->cols; c++) {
         mppt->matrix_ret->data[r][c] = 0;
         for (i = 0; i < mppt->matrix_one->cols; i++) {
            mppt->matrix_ret->data[r][c] += mppt->matrix_one->data[r][i] * mppt->matrix_two->data[i][c];
         }
      }
   }

   return NULL;
}

void swap(double *a, double *b) {
    double aux = *a;
    *a = *b;
    *b = aux;
}

int partition(double *vector, int low, int high) {
    double pivot = vector[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (vector[j] <= pivot)
        {
            i++;
            swap(&vector[i], &vector[j]);
        }
    }
    swap(&vector[i + 1], &vector[high]);
    return (i + 1);
}

void quick_sort(double *vector, int low, int high, int layer) {
   if (low < high) {
      int part = partition(vector, low, high);
      layer ++;

      deploy_quick_sort(vector, low, part - 1, layer);
      deploy_quick_sort(vector, low, part - 1, layer);
   }
}

void *sort_worker(void *arg) {
   sort_thread *sortt = (sort_thread *) arg;
   quick_sort(sortt->vector, sortt->low, sortt->high, sortt->layer);
   return NULL;
}

void deploy_quick_sort(double *vector, int low, int high, int layer) {
      if (low < high) {
      int part = partition(vector, low, high);
      if (layer < N_CPU) {
         sort_thread *addt = NULL;
         pthread_t *threads = NULL;

         sort_thread* sortt = (sort_thread*) malloc(sizeof(sort_thread) * 2);
         pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * 2);

         sortt[0].vector = vector;
         sortt[1].vector = vector;

         sortt[0].layer = layer;
         sortt[1].layer = layer;
         
         sortt[0].low = low;
         sortt[1].low = part + 1;
         
         sortt[0].high = part - 1;
         sortt[1].high = high;

         pthread_create(&threads[0], NULL, sort_worker, (void *) (sortt));
         pthread_create(&threads[1], NULL, sort_worker, (void *) (sortt + 1));
         pthread_join(threads[0], NULL);
         pthread_join(threads[1], NULL);
      }


      } else {
         quick_sort(vector, low, part - 1, layer);
         quick_sort(vector, part + 1, high, layer);
      }
   }
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
   int i;
   matrix_t* matrix_r = matrix_create(matrix_a->rows, matrix_a->cols);

   add_thread *addt = NULL;
   pthread_t *threads = NULL;

   addt = (add_thread*) malloc(sizeof(add_thread) * N_CPU);
   threads = (pthread_t*) malloc(sizeof(pthread_t) * N_CPU);

   for(i = 0; i < N_CPU; i++) {
      addt[i].id = i;
      addt[i].matrix_one = matrix_a;
      addt[i].matrix_two = matrix_b;
      addt[i].matrix_ret = matrix_r;
      pthread_create(&threads[i], NULL, add_worker, (void *) (addt + i));
   }

   for (i = 0; i < N_CPU; i++) {
		pthread_join(threads[i], NULL);		
	}

   return matrix_r;
}

matrix_t *matrix_multiply(matrix_t *matrix_a, matrix_t *matrix_b) {
   int i;

   matrix_t* matrix_r = matrix_create(matrix_a->rows, matrix_b->cols);

   mpp_thread *mppt = NULL;
   pthread_t *threads = NULL;

   mppt = (mpp_thread*) malloc(sizeof(mpp_thread) * N_CPU);
   threads = (pthread_t*) malloc(sizeof(pthread_t) * N_CPU);

   for(i = 0; i < N_CPU; i++) {
      mppt[i].id = i;
      mppt[i].matrix_one = matrix_a;
      mppt[i].matrix_two = matrix_b;
      mppt[i].matrix_ret = matrix_r;
      pthread_create(&threads[i], NULL, multiply_worker, (void *) (mppt + i));
   }

   for (i = 0; i < N_CPU; i++) {
		pthread_join(threads[i], NULL);		
	}

   return matrix_r;
}

matrix_t *matrix_sort(matrix_t *matrix_a) {
    int rows_final = matrix_a->rows;
    int cols_final = matrix_a->cols;

    matrix_t *resultado = matrix_create(rows_final, cols_final);

    memcpy(resultado->data[0], matrix_a->data[0], cols_final * rows_final * sizeof(double));
    quick_sort(resultado->data[0], 0, rows_final * cols_final - 1, 0);

    return resultado;
}

