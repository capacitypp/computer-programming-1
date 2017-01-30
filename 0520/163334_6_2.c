/*
 * 学籍番号・氏名 : 163334 佐野和輝
 * 授業回・課題番号 : 第6回・第2課題
 * 工夫した点 : 力任せ検索とBM methodにおける関数の引数や戻り値を揃え、関数ポインタとして利用しやすくした。また、ファイルサイズの取得やファイルの読み込みなどの個々の処理を関数化して、main関数が簡潔に記述できるようにした。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ASCII_SIZE	0x80

/* ファイルのサイズを取得する */
fpos_t get_file_size(FILE *fp)
{
	fpos_t sz;
	fseek(fp, 0, SEEK_END);
	fgetpos(fp, &sz);
	fseek(fp, 0, SEEK_SET);
	return sz;
}

/* 指定されたサイズ分だけファイルを読み込む */
char *read_file(FILE *fp, fpos_t size)
{
	char *str;
	/* メモリが確保できなければ終了する */
	if ((str = (char*)malloc(size)) == NULL) {
		perror("malloc() failed.\n");
		exit(1);
	}
	fread(str, sizeof(char), size, fp);
	return str;
}

/* 力任せ検索関数 */
void force_search(char *text, int filesize, char *pattern)
{
	int i;
	int n, len;
	clock_t t1, t2;

	n = 0;
	len = strlen(pattern);
	/* 処理時間を計測する */
	t1 = clock();
	for (i = 0; i < filesize; i++)
		if (strncmp(text + i, pattern, len) == 0)
				n++;
	t2 = clock();
	/* 結果を表示する */
	printf("\"%s\" found %d times!\n", pattern, n);
	printf("Used CPU time: %.10f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
}

/* パターンにおいて、指定された文字が後ろから何番目にあるかを求める */
int search_letter(char *pattern, int len, char letter)
{
	int i;
	for (i = len - 1; i >= 0; i--)
		if (pattern[i] == letter)
			return len - 1 - i;
	/* パターンに含まれなければ、パターンの長さを返す */
	return len;
}

/* BM method */
void bm_search(char *text, int filesize, char *pattern)
{
	int i;
	int n, len;
	clock_t t1, t2;
	/* アスキーコードの各文字が、パターンにおいて後ろから何番目に存在するかを格納する配列 */
	int skip[ASCII_SIZE];

	n = 0;
	len = strlen(pattern);
	t1 = clock();

	/* 配列skipの値を設定する */
	for (i = 0; i < sizeof(skip) / sizeof(skip[0]); i++)
		skip[i] = search_letter(pattern, len, (char)i);
	
	i = len - 1;
	while (i < filesize) {
		/* パターンの後ろから比較を行う */
		int j = len - 1;
		while (j >= 0) {
			if (text[i - (len - 1) + j] != pattern[j])
				break;
			j--;
		}
		/* 一致したら、カウントアップする */
		if (j < 0) {
			n++;
			i++;
		/* そうでなければ、一致しなかった文字の存在する場所に合わせる */
		} else {
			int tmp = j - (len - 1);
			int skip_idx = skip[text[i + tmp]];
			int di = skip_idx + tmp;
			if (di <= 0)
				di = 1;
			i += di;
		}
	}

	t2 = clock();
	printf("\"%s\" found %d times!\n", pattern, n);
	printf("Used CPU time: %.10f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
}

int main(int argc, char *argv[])
{
	FILE *fp;
	char *str;

	/* コマンドライン引数のチェックを行う */
	if (argc != 3) {
		perror("引数の数が正しくありません");
		return 1;
	}

	/* ファイルが開けない場合には終了する */
	if ((fp = fopen(argv[1], "r")) == NULL) {
		/* ファイルが開けない場合には終了する */
		perror("ファイルを開けません。\n");
		return 1;
	}

	/* ファイルサイズを取得する */
	fpos_t size = get_file_size(fp);
	/* ファイルを読み込む */
	str = read_file(fp, size);
	fclose(fp);

	/* 力任せ検索を行う */
	printf("----- brute-force search -----\n");
	force_search(str, size, argv[2]);
	/* BM methodを行う */
	printf("\n----- BM method -----\n");
	bm_search(str, size, argv[2]);

	/* 読み込んだテキストを解放する */
	free(str);

	return 0;
}

