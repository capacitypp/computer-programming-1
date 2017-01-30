/*
 * 学籍番号・氏名 : 163334 佐野和輝
 * 授業回・課題番号 : 第5回・第1課題
 * 工夫した点 : 関数を利用する側でメモリ確保におけるエラーを考慮しなくても済むようにした。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct triangle {
	double x1, y1;
	double x2, y2;
	double x3, y3;
	double r, g, b;
} Triangle;

typedef struct list {
	Triangle* triangle;
	struct list *next;
} LIST;

/* Triangleを生成する */
Triangle *allocTriangle(void)
{
	Triangle *triangle;
	if ((triangle = (Triangle*)malloc(sizeof(Triangle))) == NULL) {
		fprintf(stderr, "malloc() failed");
		exit(1);
	}
	return triangle;
}

/* Listを生成する */
LIST *allocList(Triangle *triangle)
{
	LIST *ptr;

	if ((ptr = (LIST*)malloc(sizeof(LIST))) == NULL) {
		fprintf(stderr, "malloc() failed");
		exit(1);
	}
	ptr->triangle = triangle;
	ptr->next = NULL;

	return ptr;
}

/* リストに挿入する */
LIST *insertList(LIST *list, Triangle *triangle)
{
	if (list == NULL) {
		/* Listを生成する */
		list = allocList(triangle);
		return list;
	}
	list->next = insertList(list->next, triangle);
	return list;
}

/* リストを解放する */
void freeList(LIST *list)
{
	if (list == NULL)
		return;
	freeList(list->next);
	free(list->triangle);
	free(list);
}

/* 0.0~1.0の乱数を生成する */
double doubleRandom(void)
{
	return (double)rand() / (double)RAND_MAX;
}

/* x座標の値を生成する */
double randomX(void)
{
	return doubleRandom() * 500 + 50.0;
}

/* y座標の値を生成する */
double randomY(void)
{
	return doubleRandom() * 600 + 50.0;
}

/* 乱数をもとにTriangleを生成する */
Triangle *randomTriangle(void)
{
	Triangle *triangle = allocTriangle();
	triangle->r = doubleRandom();
	triangle->g = doubleRandom();
	triangle->b = doubleRandom();
	triangle->x1 = randomX();
	triangle->x2 = randomX();
	triangle->x3 = randomX();
	triangle->y1 = randomY();
	triangle->y2 = randomY();
	triangle->y3 = randomY();
	return triangle;
}

/* Listの内容をファイルに出力する */
void writeList(FILE *fp, LIST *list)
{
	if (list == NULL) {
		fprintf(fp, "showpage\n");
		return;
	}
	Triangle *triangle = list->triangle;
	fprintf(fp, "%f %f %f setrgbcolor\n", triangle->r, triangle->g, triangle->b);
	fprintf(fp, "newpath\n");
	fprintf(fp, "%f %f moveto\n", triangle->x1, triangle->y1);
	fprintf(fp, "%f %f lineto\n", triangle->x2, triangle->y2);
	fprintf(fp, "%f %f lineto\n", triangle->x3, triangle->y3);
	fprintf(fp, "closepath\n");
	fprintf(fp, "stroke\n");

	/* 次の内容について再帰的に処理する */
	writeList(fp, list->next);
}

int main(void)
{
	int i;
	LIST *list = NULL;
	FILE *fp;
	int n;

	if ((fp = fopen("163334_5_1.ps", "w")) == NULL) {
		fprintf(stderr, "fopen() failed");
		exit(1);
	}

	/* 乱数のseedを時間をもとに設定する */
	srand((unsigned)time(NULL));

	n = rand() % 98 + 3;
	/* n個のTriangleを生成する */
	for (i = 0; i < n; i++) {
		Triangle *triangle = randomTriangle();
		list = insertList(list, triangle);
	}

	writeList(fp, list);

	freeList(list);

	fclose(fp);

	return 0;
}

