/*
 * 学籍番号・氏名 : 163334 佐野和輝
 * 授業回・課題番号 : 第6回・第1課題
 * 工夫した点 : 要素の入れ替え処理を別の関数として実装することによって、ソースコードの可読性がよくなるようにした。
 */

#include <stdlib.h>

/* ptr1とptr2の指す領域の値を入れ替える */
void swap(char *ptr1, char *ptr2, size_t width)
{
	int i;
	for (i = 0; i < width; i++) {
		char tmp = ptr1[i];
		ptr1[i] = ptr2[i];
		ptr2[i] = tmp;
	}
}

/* バブルソート関数 */
void bubblesort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *))
{
	int i;
	char *char_base = (char*)base;
	/* ソート対象配列の一番後ろの要素を指すポインタ */
	char *ptr = char_base + (nel - 1) * width;

	/* 先頭からソート対象の領域を狭くしていく */
	for (i = 1; i < nel; i++) {
		int j;
		char *ptr2 = ptr;
		/* ptr2の一つ前の要素を指すポインタ */
		char *ptr1 = ptr2 - width;

		/* ソート対象の領域の先頭に最小値を移動させる */
		for (j = nel - 1; j >= i; j--) {
			/* 順番が間違っていたら、入れ替える */
			if (compar((const void*)ptr1, (const void*)ptr2) > 0)
				swap(ptr1, ptr2, width);
			/* 一つ前の要素を指すように更新する */
			ptr1 -= width;
			ptr2 -= width;
		}
	}
}

