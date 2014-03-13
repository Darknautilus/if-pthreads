#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, const char **argv)
{
	const char *str;
	char *endptr;

	FILE *numbers = fopen("numbers","r");
	if(numbers == NULL)
	{
		printf("Fichier numbers introuvable\n");
		exit(EXIT_FAILURE);
	}
	
	uint64_t num;
	while(fscanf(numbers,"%llu\n",&num) != EOF)
	{
		print_prime_factors(num);
		printf("\n");
	}

	fclose(numbers);

	return 0;
}
