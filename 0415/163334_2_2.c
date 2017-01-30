/*
 * 学籍番号・氏名 : 163334 佐野和輝
 * 授業回・課題番号 : 第2回・第2課題
 * 工夫した点 : 探す対象の文字列を配列に格納してループでカウントすることによって、簡潔に記述した。
 */

#include <stdio.h>

#define BUF_SIZE	256

/* 文字列の長さをカウントする */
int strlength(char *str)
{
	char *ptr = str;
	/* 改行コードも含めて数える */
	while (*str != '\0')
			str++;
	return str - ptr;
}

/* 文字列が等しいか比較する */
int strcompare(char *str1, char *str2)
{
	/* 短い文字列の文字数分だけ比較する */
	while (*str1 != '\0' && *str2 != '\0')
			if (*str1++ != *str2++)
					return 1;
	return 0;
}

/* 1行に含まれる文字列keyの個数をカウントする */
int count(char *str, char *key)
{
	int i;
	int cnt = 0;
	int str_length = strlength(str);
	int key_length = strlength(key);

	/* 1文字ずつずらしながら比較する */
	for (i = 0; i < str_length - key_length + 1; i++)
			if (!strcompare(&str[i], key))
					cnt++;

	return cnt;
}

int main(void)
{
	int i;
	FILE *fp;
	char str[BUF_SIZE];

	/* カウントに使用する配列 */
	int cnt[3] = {0};
	/* 探す対象の文字列 */
	char key[][BUF_SIZE] = {"Alice", "wonder", "self"};

	/* ファイルが開けなければ終了する */
	if ((fp = fopen("Alice_in_Wonderland.txt", "r")) == NULL) {
		fprintf(stderr, "ファイルを開けません。\n");
		return 1;
	}

	while(fgets(str, sizeof(str), fp) != NULL) {
			for (i = 0; i < sizeof(cnt) / sizeof(cnt[0]); i++)
					cnt[i] += count(str, key[i]);
	}

	/* 結果を表示する */
	for (i = 0; i < sizeof(cnt) / sizeof(cnt[0]); i++)
			printf("%s : %d\n", key[i], cnt[i]);

	fclose(fp);

	return 0;
}

/*
実行結果:

$ gcc -o 163334_2_2 163334_2_2.c 
$ ./163334_2_2
Alice : 395
wonder : 28
self : 120
$

*/

