#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

unsigned short int is_prime(uint64_t p)
{
	uint64_t i;
	if(p<=1)
		return 0;
	for(i=2;i*i<=p;i++)
	{
		if(!(p % i))
			return 0;
	}
	return 1;
}

void print_prime_factors(uint64_t n)
{
	printf("%llu: ",n);
	uint64_t div = 2;
	while(n)
	{
		if(n%div)
		{
			div++;
		}
		else
		{
			n = n/div;
			printf("%llu ",div);
			if(n==1)
				break;
		}
	}
}

void *printer_thread(void *n)
{
	print_prime_factors(*((uint64_t*)n));
	printf("\n");
}

typedef struct
{
	FILE *file;
	pthread_mutex_t *mutex;
} threadFile;

void *reader_thread(void *threadfile)
{
	uint64_t num;
	
	threadFile *tf = (threadFile*)threadfile;

	pthread_mutex_lock(tf->mutex);
	while(fscanf(tf->file,"%llu\n",&num) != EOF)
	{
		pthread_mutex_unlock(tf->mutex);
		print_prime_factors(num);
		printf("\n");
		pthread_mutex_lock(tf->mutex);
	}
	pthread_mutex_unlock(tf->mutex);
}

int main(int argc, const char **argv)
{
	FILE *numbers = fopen("numbers","r");
	if(numbers == NULL)
	{
		printf("Fichier numbers introuvable\n");
		exit(EXIT_FAILURE);
	}

	uint8_t is_parallel = 1;
	if(argc > 1 && !strcmp(argv[1],"-s"))
	{
		is_parallel = 0;
		printf("Runs in sequential mode\n");
	}
	else
	{
		printf("Runs in multithreaded mode\n");
	}
	
	uint64_t num1;
	uint64_t num2;
	pthread_t thread1;
	pthread_t thread2;

	if(is_parallel)
	{
		pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

		threadFile tf;
		tf.file = numbers;
		tf.mutex = &fileMutex;

		pthread_create(&thread1,NULL,reader_thread,&tf);
		pthread_create(&thread2,NULL,reader_thread,&tf);
		
		pthread_join(thread2,NULL);
		pthread_join(thread1,NULL);
	}
	else
	{
		while(fscanf(numbers,"%llu\n",&num1) != EOF)
		{
			print_prime_factors(num1);
			printf("\n");
		}
	}

	fclose(numbers);

	return 0;
}
