#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  FILE *fp_t, *fp_b;
  int xsize, ysize, x, y;

  // 引数チェック
  if (argc != 3) {
    fprintf(stderr, "USAGE: %s text_data_file_name binary_data_file_name\n", argv[0]);
  }

  // ファイルオープン
  fp_t = fopen(argv[1], "r");
  fp_b = fopen(argv[2], "wb");

  // 座標の最大値
  fscanf(fp_t, "%d", &xsize);
  fscanf(fp_t, "%d", &ysize);

  fwrite(&xsize, sizeof(int), 1, fp_b);
  fwrite(&ysize, sizeof(int), 1, fp_b);

  // 座標データ
  while (fscanf(fp_t, "%d", &x) != EOF) {
    if (fscanf(fp_t, "%d", &y) == EOF) {
      fprintf(stderr, "Data format error!\n");
      exit(EXIT_FAILURE);
    }
    fwrite(&x, sizeof(int), 1, fp_b);
    fwrite(&y, sizeof(int), 1, fp_b);
  }

  // ファイルクローズ
  fclose(fp_t);
  fclose(fp_b);

  return EXIT_SUCCESS;
}
