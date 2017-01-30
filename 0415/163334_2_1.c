/*
 * 学籍番号・氏名 : 163334 佐野和輝
 * 授業回・課題番号 : 第2回・第1課題
 * 工夫した点 : ファイルが開けない場合に正しく終了できるようにした。また、改行コードが無い最終行に対しても正しく動作するようにした。
 */

#include <stdio.h>

#define BUF_SIZE	256

/* 文字列の文字数を求める */
int strlength(char *str)
{
	char *ptr = str;
	/* 改行をカウントしない */
	while (*str != '\0' && *str != '\n')
			str++;
	return str - ptr;
}

/* 文字列を逆順にする */
void reverse(char *str)
{
	int i;
	int length = strlength(str);

	for (i = 0; i < length / 2; i++) {
		/* 文字を入れ替える */
		char tmp = str[i];
		str[i] = str[length - i - 1];
		str[length - i - 1] = tmp;
	}
}

int main(void)
{
	FILE *fp;
	char str[BUF_SIZE];

	if ((fp = fopen("Alice_in_Wonderland.txt", "r")) == NULL) {
		/* ファイルが開けない場合には終了する */
		fprintf(stderr, "ファイルを開けません。\n");
		return 1;
	}

	while(fgets(str, sizeof(str), fp) != NULL) {
		reverse(str);
		printf("%s", str);
	}

	fclose(fp);

	return 0;
}

/*
実行結果:

$ gcc -o 163334_2_1 163334_2_1.c 
$ ./163334_2_1
DNALREDNOW NI SERUTNEVDA S'ECILA                

llorraC siweL                          

0.3 NOITIDE MURCLUF MUINNELLIM EHT               




I RETPAHC                            

eloH-tibbaR eht nwoD                      


retsis reh yb gnittis fo derit yrev teg ot gninnigeb saw ecilA  
dah ehs eciwt ro ecno  :od ot gnihton gnivah fo dna ,knab eht no
on dah ti tub ,gnidaer saw retsis reh koob eht otni depeep
',koob a fo esu eht si tahw dna` ,ti ni snoitasrevnoc ro serutcip
'?noitasrevnoc ro serutcip tuohtiw` ecilA thguoht


*/

