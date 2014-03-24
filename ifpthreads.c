#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MAX_FACTORS 50
#define MAX_DECOMP 2000000

typedef struct
{
	uint64_t num;
	int nb_factors;
	uint64_t factors[MAX_FACTORS];
} decomp;

static int nb_decomp = 0;
static decomp *known_decomp[MAX_DECOMP];
static pthread_mutex_t decomp_mutex = PTHREAD_MUTEX_INITIALIZER;

// Attention, non thread-safe
int is_known(uint64_t n/*,int *ou_ajouter*/)
{
	printf("hu\n");
	if(nb_decomp)
	{
		printf("wow");
		int i=nb_decomp/2;
		int sortir =0;
		int avant;

		while(!sortir)
		{
			avant=i;
			if(known_decomp[i]->num == n)
			{
				return i;
			}
			else if(n<known_decomp[i]->num)
			{
				i-=(i/2);
			}
			else
			{
				i+=i/2;
			}
			if(!(avant-i))
				sortir=1;
		}
	}
	return -1; // si non trouvé
}

void addDecompTriee(decomp *d)
{			
	int i=1;
	for (i; i<=nb_decomp;i++)
	{
		if(d->num<known_decomp[i-1]->num)
		{
			int j=nb_decomp;
			for(j;j>i;j--)
			{
				known_decomp[j]=known_decomp[j-1];
			}
			known_decomp[i]=d;
			nb_decomp++;
			break;
		}
	}
}

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

int get_prime_factors(uint64_t n, uint64_t *dest)
{
	uint8_t i = 0;
	uint8_t pas = 1;
	uint64_t div = 2;
	decomp *d = (decomp*)malloc(sizeof(decomp));
	d->num = n;
	while(div*div<=n)
	{
		if(n%div)
		{
			div += pas;
			if(div > 2)
			{
				pas = 2;
			}
		}
		else
		{
			n = n/div;
			dest[i] = div;
			d->factors[i] = div;
			i++;
			if(n==1)
				break;
		}
	}
	d->nb_factors = i;

	pthread_mutex_lock(&decomp_mutex);
	//known_decomp[nb_decomp] = d;
	addDecompTriee(d);
	nb_decomp++;
	pthread_mutex_unlock(&decomp_mutex);

	return i;
}

void print_prime_factors(uint64_t n)
{
	uint64_t *factors;

	int j,k;
	int n_known = 0;

	pthread_mutex_lock(&decomp_mutex);
	if((k = is_known(n)) >= 0)
	{
		n_known = 1;
		factors = known_decomp[k]->factors;
		k = known_decomp[k]->nb_factors;
		pthread_mutex_unlock(&decomp_mutex);
	}
	else
	{
		pthread_mutex_unlock(&decomp_mutex);
		factors = (uint64_t *)malloc(MAX_FACTORS*sizeof(uint64_t));
		k = get_prime_factors(n,factors);
	}

	printf("%llu: ",n);
	for(j=0;j<k;j++)
	{
		printf("%llu ",factors[j]);
	}

	if(!n_known)
		free(factors);
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
	pthread_t thread3;
	pthread_t thread4;

	if(is_parallel)
	{
		pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

		threadFile tf;
		tf.file = numbers;
		tf.mutex = &fileMutex;

		pthread_create(&thread1,NULL,reader_thread,&tf);
		pthread_create(&thread2,NULL,reader_thread,&tf);
		pthread_create(&thread3,NULL,reader_thread,&tf);
		pthread_create(&thread4,NULL,reader_thread,&tf);
		
		pthread_join(thread4,NULL);
		pthread_join(thread3,NULL);
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

	int i;
	for(i=0;i<nb_decomp;i++)
	{
		free(known_decomp[i]);
	}

	return 0;
}
