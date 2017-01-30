/*
 * 学籍番号・氏名 : 163334 佐野和輝
 * 授業回・課題番号 : 第3回・第1課題
 * 工夫した点 : ファイルが開けない場合など、エラーの場合に正しく終了できるようにした。また、2次元配列の確保などの頻繁に行う処理を関数化して、プログラムを簡潔に記述した。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE	256

/* double型の1次元配列を確保する */
double *malloc_double_array(int n)
{
	double *array;
	if ((array = (double*)malloc(n * sizeof(double))) == NULL) {
		perror("配列の確保に失敗しました。");
		exit(1);
	}
	return array;
}

/* double型の2次元配列を確保する */
double **malloc_double_array2(int n, int m)
{
	int i;
	double **array;
	if ((array = (double**)malloc(n * sizeof(double*))) == NULL) {
		perror("配列の確保に失敗しました。");
		exit(1);
	}
	for (i = 0; i < n; i++) {
			array[i] = malloc_double_array(m);
	}
	return array;
}

/* datファイルの1行目からフレーム数と特徴次元数を取り出す */
void get_frames_and_dimension(char str[], int *frames, int *dimension)
{
	char *ptr = strtok(str, " ");
	*frames = atoi(ptr);
	ptr = strtok(NULL, " ");
	*dimension = atoi(ptr);
}

/* 1行分のフレームからベクトルを取り出す */
void frame2data_array(char str[], int dimension, double *data_array)
{
	char *ptr = strtok(str, " ");
	while (dimension-- > 0) {
		*data_array++ = atof(ptr);
		ptr = strtok(NULL, " ");
	}
}

/* datファイルを読み込む */
double **read_data(FILE *fp, int *frames, int *dimension)
{
	int i;
	char str[BUF_SIZE];
	double **data_array;

	/* フレーム数と特徴次元数を読み込む */
	if(fgets(str, sizeof(str), fp) == NULL) {
		perror("フレーム数/特徴次元数の読み込みに失敗しました。");
		exit(1);
	}

	get_frames_and_dimension(str, frames, dimension);

	data_array = malloc_double_array2(*frames, *dimension);
	/* データ本体を読み込む */
	for (i = 0; i < *frames; i++) {
		if(fgets(str, sizeof(str), fp) == NULL) {
			perror("フレームの読み込みに失敗しました。");
			exit(1);
		}
		frame2data_array(str, *dimension, data_array[i]);
	}

	return data_array;
}

/* 平均ベクトルを求める */
double *get_mean(int ndim, double **data, int ndata)
{
	int i;
	double *mean = malloc_double_array(ndim);

	for (i = 0; i < ndim; i++) {
		int j;
		mean[i] = 0.0;
		for (j = 0; j < ndata; j++)
			mean[i] += data[j][i];
		mean[i] /= ndata;
	}

	return mean;
}

/* 分散共分散行列の(i,j)要素を求める */
double calc_sigma2(double **data, int ndata, double *mean, int i, int j)
{
	int n;
	double sigma2 = 0.0;

	for (n = 0; n < ndata; n++)
			sigma2 += (data[n][i] - mean[i]) * (data[n][j] - mean[j]);
	sigma2 /= ndata;

	return sigma2;
}

/* 分散共分散行列を求める */
double **get_cova(int ndim, double **data, int ndata)
{
	int i, j;
	double **cova = malloc_double_array2(ndim, ndim);
	double *mean= get_mean(ndim, data, ndata);

	for (i = 0; i < ndim; i++)
	for (j = 0; j < ndim; j++)
		cova[i][j] = calc_sigma2(data, ndata, mean, i, j);

	return cova;
}

/* double型の2次元配列を解放する */
void free_data_array2(double **data_array, int frames)
{
	int i;
	for (i = 0; i < frames; i++)
			free(data_array[i]);
	free(data_array);
}

int main(void)
{
	FILE *fp;
	double **data_array;
	int frames, dimension;
	double *mean, **cova;
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
	mean = get_mean(dimension, data_array, frames);
	/* 分散共分散行列を求める */
	cova = get_cova(dimension, data_array, frames);

	/* 計算結果を表示する */
	puts("mean");
	for (i = 0; i < dimension; i++)
			printf("%g\n", mean[i]);
	putchar('\n');
	puts("cova");
	for (i = 0; i < dimension; i++) {
		int j;
		for (j = 0; j < dimension; j++)
			printf("%f ", cova[i][j]);
		putchar('\n');
	}

	/* 確保した領域を解放する */
	free_data_array2(data_array, frames);
	free(mean);
	free_data_array2(cova, dimension);

	return 0;
}

/*
実行結果:

$ gcc -o 163334_3_1 163334_3_1.c 
$ ./163334_3_1
mean
-1.41388e-05
-1.28535e-06
-1.79949e-05
-8.99743e-06
-1.15681e-05
1.9065e-16
-1.54242e-05
-2.55722e-16
6.42674e-06
-3.85604e-06

cova
29.431324 -5.025587 2.005196 -21.295439 -15.806846 8.677714 -5.474325 -3.670951 -9.022653 -1.762498 
-5.025587 47.686228 2.810142 12.495066 16.664772 14.129448 23.788095 -16.657507 -14.308402 3.179601 
2.005196 2.810142 46.322844 21.025262 -27.958765 -5.785768 -11.688003 12.296703 -1.795753 -14.474213 
-21.295439 12.495066 21.025262 86.918294 27.923627 -25.795114 9.137526 8.794512 13.922046 12.330206 
-15.806846 16.664772 -27.958765 27.923627 92.341439 3.414691 22.661727 -0.295393 13.878921 32.496726 
8.677714 14.129448 -5.785768 -25.795114 3.414691 65.100650 19.774264 -22.028061 -10.369529 2.569036 
-5.474325 23.788095 -11.688003 9.137526 22.661727 19.774264 74.811341 -11.726446 -2.255973 -0.155340 
-3.670951 -16.657507 12.296703 8.794512 -0.295393 -22.028061 -11.726446 65.449316 18.735012 0.063330 
-9.022653 -14.308402 -1.795753 13.922046 13.878921 -10.369529 -2.255973 18.735012 59.501617 0.669178 
-1.762498 3.179601 -14.474213 12.330206 32.496726 2.569036 -0.155340 0.063330 0.669178 42.746026 

*/

