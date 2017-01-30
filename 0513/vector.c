#include <stdio.h>
#include <stdlib.h>

#include "vecmat.h"

/* Vectorを生成する */
Vector *new_vector(int n)
{
	int i;
	Vector *vector;
	if ((vector = (Vector*)malloc(sizeof(Vector))) == NULL) {
		fprintf(stderr, "malloc() failed.\n");
		exit(-1);
	}
	vector->n = n;
	if ((vector->v = (double*)malloc(sizeof(double) * n)) == NULL) {
		fprintf(stderr, "malloc() failed.\n");
		exit(-1);
	}
	/* 要素を0で初期化する */
	for (i = 0; i < n; i++)
		vector->v[i] = 0.0;
	return vector;
}

/* Vectorを解放する */
void free_vector(Vector *vector)
{
	free(vector->v);
	free(vector);
}

/* Vectorを表示する */
void vecprint(Vector *vector)
{
	int i;
	for (i = 0; i < vector->n; i++)
		printf("%f\n", vector->v[i]);
}

