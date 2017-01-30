/*
 * 学籍番号・氏名 : 163334 佐野和輝
 * 授業回・課題番号 : 第3回・第1課題
 * 工夫した点 : ファイルの読み込みにおいても積極的にVectorやMatrixを利用すうようにした。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vecmat.h"

#define BUF_SIZE	256

/* datファイルの1行目からフレーム数と特徴次元数を取り出す */
void get_frames_and_dimension(char str[], int *frames, int *dimension)
{
	char *ptr = strtok(str, " ");
	*frames = atoi(ptr);
	ptr = strtok(NULL, " ");
	*dimension = atoi(ptr);
}

/* 1行分のフレームからベクトルを取り出す */
Vector *frame2data_array(char str[], int dimension)
{
	int i;
	Vector *vector = new_vector(dimension);
	char *ptr = strtok(str, " ");
	for (i = 0; i < vector->n; i++) {
		vector->v[i] = atof(ptr);
		ptr = strtok(NULL, " ");
	}
	return vector;
}

/* datファイルを読み込む */
Matrix *read_data(FILE *fp, int *frames, int *dimension)
{
	int i;
	char str[BUF_SIZE];
	Matrix *matrix;

	/* フレーム数と特徴次元数を読み込む */
	if(fgets(str, sizeof(str), fp) == NULL) {
		perror("フレーム数/特徴次元数の読み込みに失敗しました。");
		exit(1);
	}

	get_frames_and_dimension(str, frames, dimension);
	matrix = new_matrix(*frames, *dimension);
	/* データ本体を読み込む */
	for (i = 0; i < *frames; i++) {
		int j;
		if(fgets(str, sizeof(str), fp) == NULL) {
			perror("フレームの読み込みに失敗しました。");
			exit(1);
		}
		Vector *data_array = frame2data_array(str, *dimension);
		for (j = 0; j < data_array->n; j++)
			matrix->t[i][j] = data_array->v[j];
		free_vector(data_array);
	}

	return matrix;
}

/* 平均ベクトルを求める */
Vector *get_mean(Matrix *matrix)
{
	int i;
	Vector *mean = new_vector(matrix->m);

	for (i = 0; i < matrix->m; i++) {
		int j;
		mean->v[i] = 0.0;
		for (j = 0; j < matrix->n; j++)
			mean->v[i] += matrix->t[j][i];
		mean->v[i] /= matrix->n;
	}

	return mean;
}

/* 分散共分散行列の(i,j)要素を求める */
double calc_sigma2(Matrix *matrix, Vector *mean, int i, int j)
{
	int n;
	double sigma2 = 0.0;

	for (n = 0; n < matrix->n; n++)
			sigma2 += (matrix->t[n][i] - mean->v[i]) * (matrix->t[n][j] - mean->v[j]);
	sigma2 /= matrix->n;

	return sigma2;
}

/* 分散共分散行列を求める */
Matrix *get_cova(Matrix *matrix)
{
	int i, j;
	Matrix *cova = new_matrix(matrix->m, matrix->m);
	Vector *mean = get_mean(matrix);

	for (i = 0; i < matrix->m; i++)
	for (j = 0; j < matrix->m; j++)
		cova->t[i][j] = calc_sigma2(matrix, mean, i, j);

	return cova;
}

int main(void)
{
	FILE *fp;
	Matrix *data_array;
	int frames, dimension;
	Vector *mean;
	Matrix *cova;
	int i;

	if ((fp = fopen("A01M0007-0007.dat", "r")) == NULL) {
		/* ファイルが開けない場合には終了する */
		perror("ファイルを開けません。\n");
		return 1;
	}

	/* datファイルを読み込む */
	data_array = read_data(fp, &frames, &dimension);

	fclose(fp);

	/* 平均ベクトルを求める */
	mean = get_mean(data_array);
	/* 分散共分散行列を求める */
	cova = get_cova(data_array);

	/* 計算結果を表示する */
	puts("mean");
	vecprint(mean);
	puts("cova");
	matprint(cova);

	/* 確保した領域を解放する */
	free_matrix(data_array);
	free_vector(mean);
	free_matrix(cova);

	return 0;
}

