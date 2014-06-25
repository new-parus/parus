#include <stdlib.h>

double my_rand(void)
{
	return random()/(double)RAND_MAX;
}

