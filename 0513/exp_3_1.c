/*
 * 学籍番号・氏名：143399 技科大太郎
 * 授業回・課題番号：第3回・第1課題
 * 工夫した点：mallocをエラー処理を含めた関数にした
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRLEN 512

/*
 * エラー処理を含めたmalloc()関数
 * 入出力：malloc()と同じ
 */
void *myMalloc(size_t size)
{
  void *ptr;

  ptr = malloc(size);
  if (ptr == NULL) {
    fprintf(stderr, "Memory allocation error!\n");
    exit(EXIT_FAILURE);
  }

  return ptr;
}


/*
 * 平均ベクトルを求めて、領域を返す関数
 * 入力：データの次元数、データ（2次元配列）へのポインタ、データ数
 * 出力：平均ベクトルへのポインタ
 */
double *get_mean(int ndim, double **data, int ndata)
{
  int i, j;
  double *mean;

  mean = (double*)myMalloc(ndim * sizeof(double));

  for (i = 0; i < ndim; i++) {
    mean[i] = 0.0;
    for (j = 0; j < ndata; j++) {
      mean[i] += data[j][i];
    }
    mean[i] /= (double)ndata;
  }

  return mean;
}


/*
 * 分散共分散行列を求めて、領域を返す関数
 * 入力：データの次元数、データ（2次元配列）へのポインタ、データ数
 * 出力：分散共分散行列へのポインタ
 */
double **get_cova(int ndim, double **data, int ndata)
{
  int i, j, k;
  double **cova, *mean;

  cova = (double**)myMalloc(ndim * sizeof(double*));
  for (i = 0; i < ndim; i++) {
    cova[i] = (double*)myMalloc(ndim * sizeof(double));
  }

  mean = get_mean(ndim, data, ndata);  // 計算に必要なので平均を求める

  for (i = 0; i < ndim; i++) {
    for (j = 0; j < ndim; j++) {
      cova[i][j] = 0.0;
      for (k = 0; k < ndata; k++) {
	cova[i][j] += (data[k][i] - mean[i]) * (data[k][j] - mean[j]);
      }
      cova[i][j] /= (double)ndata;
    }
  }

  free(mean);  // メモリリークにならないように解放しておく

  return cova;
}


int main()
{
  FILE *fp;
  char filename[] = "A01M0007-0007.dat";  // ファイル名
  char str[STRLEN], *p;
  char *sep = " ";

  int ndata, ndim, i, j;
  double **data;

  double *mean, **cova;

  fp = fopen(filename, "r");  // ファイルを開く
  if (fp == NULL) {
    fprintf(stderr, "File Open Error: %s\n", filename);
    exit(EXIT_FAILURE);
  }

  if (fgets(str, STRLEN, fp) == NULL) {
    fprintf(stderr, "Data read error!\n");
    exit(EXIT_FAILURE);
  }
  ndata = atoi(strtok(str, sep));
  ndim = atoi(strtok(NULL, sep));
  if (ndata <= 0 || ndim <= 0) {
    fprintf(stderr, "Data error!\n");
    exit(EXIT_FAILURE);
  }

  data = (double**)myMalloc(sizeof(double*) * ndata);  // データ用２次元配列確保
  for (i = 0; i < ndata; i++) {
    data[i] = (double*)myMalloc(sizeof(double) * ndim);
  }

  for (i = 0; i < ndata; i++) {
    if (fgets(str, STRLEN, fp) == NULL) {  // fgets()でファイルから1行ずつ読み込む
      fprintf(stderr, "Data read error!\n");
      exit(EXIT_FAILURE);
    }
    if ((p = strtok(str, sep)) != NULL) {  // １つ目のデータ
      data[i][0] = atof(p);
    }
    for (j = 1; j < ndim; j++) {
      if ((p = strtok(NULL, sep)) != NULL) {  // ２つ目以降のデータ
	data[i][j] = atof(p);
      }
    }
  }

  fclose(fp);  // 読み終わったらファイルを閉じておく

  /*
  printf("Original data:\n");
  for (i = 0; i < ndata; i++) {
    for (j = 0; j < ndim; j++) {
      printf("%.3f ", data[i][j]);
    }
    printf("\n");
  }
  printf("\n");
  */

  mean = get_mean(ndim, data, ndata);
  cova = get_cova(ndim, data, ndata);

  printf("Mean vector:\n");
  for (i = 0; i < ndim; i++) {
    printf("%7.3f ", mean[i]);
  }
  printf("\n\n");

  printf("Covariance matrix:\n");
  for (i = 0; i < ndim; i++) {
    for (j = 0; j < ndim; j++) {
      printf("%7.3f ", cova[i][j]);
    }
    printf("\n");
  }

  free(mean);
  for (i = 0; i < ndim; i++) {
    free(cova[i]);
  }
  free(cova);

  return EXIT_SUCCESS;
}

/*
実行結果：

kyama% gcc -Wall -o exp_3_1 exp_3_1.c
kyama% ./exp_3_1
Mean vector:
-0.0 -0.0 -0.0 -0.0 -0.0 0.0 -0.0 -0.0 0.0 -0.0 

Covariance matrix:
29.431 -5.026 2.005 -21.295 -15.807 8.678 -5.474 -3.671 -9.023 -1.762 
-5.026 47.686 2.810 12.495 16.665 14.129 23.788 -16.658 -14.308 3.180 
2.005 2.810 46.323 21.025 -27.959 -5.786 -11.688 12.297 -1.796 -14.474 
-21.295 12.495 21.025 86.918 27.924 -25.795 9.138 8.795 13.922 12.330 
-15.807 16.665 -27.959 27.924 92.341 3.415 22.662 -0.295 13.879 32.497 
8.678 14.129 -5.786 -25.795 3.415 65.101 19.774 -22.028 -10.370 2.569 
-5.474 23.788 -11.688 9.138 22.662 19.774 74.811 -11.726 -2.256 -0.155 
-3.671 -16.658 12.297 8.795 -0.295 -22.028 -11.726 65.449 18.735 0.063 
-9.023 -14.308 -1.796 13.922 13.879 -10.370 -2.256 18.735 59.502 0.669 
-1.762 3.180 -14.474 12.330 32.497 2.569 -0.155 0.063 0.669 42.746 

kyama%
*/
