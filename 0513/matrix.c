#include <stdio.h>
#include <stdlib.h>

#include "vecmat.h"

/* Matrixを生成する */
Matrix *new_matrix(int n, int m)
{
	int i, j;
	Matrix *matrix;
	if ((matrix = (Matrix*)malloc(sizeof(Matrix))) == NULL) {
		fprintf(stderr, "malloc() failed.\n");
		exit(-1);
	}
	matrix->n = n;
	matrix->m = m;
	if ((matrix->t = (double**)malloc(sizeof(double*) * n)) == NULL) {
		fprintf(stderr, "malloc() failed.\n");
		exit(-1);
	}
	for (i = 0; i < n; i++) {
		if ((matrix->t[i] = (double*)malloc(sizeof(double) * m)) == NULL) {
			fprintf(stderr, "malloc() failed.\n");
			exit(-1);
		}
		/* 要素を0で初期化する */
		for (j = 0; j < m; j++)
			matrix->t[i][j] = 0.0;
	}
	return matrix;
}

/* Matrixを解放する */
void free_matrix(Matrix *matrix)
{
	int i;
	for (i = 0; i < matrix->n; i++)
		free(matrix->t[i]);
	free(matrix);
}

/* Matrixを表示する */
void matprint(Matrix *matrix)
{
	int i, j;
	for (i = 0; i < matrix->n; i++) {
		for (j = 0; j < matrix->m; j++)
			printf("%f ", matrix->t[i][j]);
		printf("\n");
	}
}

