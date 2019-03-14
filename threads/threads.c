#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int id;
} DadosThread;

void *trabalho_da_thread(void *);

int main(int argc, char *argv[])
{
	int num_threads = 0;	
	int i;

	if (argc < 2) {
		printf("informe: %s <num_threads>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	num_threads = atoi(argv[1]);

	printf("num_threads = %d\n", num_threads);

	DadosThread *dt = NULL;
	pthread_t *threads = NULL;

	if (!(dt = (DadosThread *) malloc(sizeof(DadosThread) * num_threads))) {
		printf("Erro ao alocar memória\n");
		exit(EXIT_FAILURE);
	}

	if (!(threads = (pthread_t *) malloc(sizeof(pthread_t) * num_threads))) {
		 printf("Erro ao alocar memória\n");
                exit(EXIT_FAILURE);
	}

	for (i = 0; i < num_threads; i++) {
		dt[i].id = i;		
		pthread_create(&threads[i], NULL, trabalho_da_thread, (void *) (dt + i));
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);		
	}
	free(dt);
	free(threads);
	return EXIT_SUCCESS;
}

void *trabalho_da_thread(void *arg)
{
	DadosThread *p = (DadosThread *) arg;
	
	printf("Thread %d executando\n", p->id);

	return NULL;
}

