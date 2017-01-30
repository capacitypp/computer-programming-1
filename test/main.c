#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double getRandom(double min, double max)
{
	return min + (rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

int main(void)
{
	int n;
	printf("%d\n", time(NULL));
	srand((unsigned long)time(NULL));
	n = (int)getRandom((double)10, (double)100);

	printf("%d\n", n);

	return 0;
}

