/*
 * 学籍番号・氏名 : 163334 佐野和輝
 * 授業回・課題番号 : 第4回・第1課題
 * 工夫した点 : 2分木を解放する関数を実装し、実際に利用するようにした。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE	256

typedef struct node {
	char *key;
	unsigned count;
	struct node *left;
	struct node *right;
} NODE;

/* Nodeを生成する */
NODE *allocNode(char *key)
{
	NODE *ptr;

	if ((ptr = (NODE*)malloc(sizeof(NODE))) == NULL) {
		fprintf(stderr, "malloc() failed");
		exit(1);
	}
	/* keyの文字数分だけ領域を確保する */
	if ((ptr->key = (char*)malloc(sizeof(char) * (strlen(key) + 1))) == NULL) {
		fprintf(stderr, "malloc() failed");
		exit(1);
	}
	/* キーを格納する */
	strcpy(ptr->key, key);
	ptr->count = 0;
	ptr->left = NULL;
	ptr->right = NULL;

	return ptr;
}

/* 2分木にNodeを挿入する */
NODE *insertNode(NODE *node, char *key)
{
	/* 同じキーを持つノードが存在しない場合 */
	if (node == NULL) {
		/* Nodeを生成する */
		node = allocNode(key);
		node->count++;
		return node;
	}
	int let = strcmp(node->key, key);
	/* 同じキーを持っていたらカウントを増やす
	 * そうでなければ再帰的に処理する */
	if (!let)
		node->count++;
	else if (let > 0)
		node->left = insertNode(node->left, key);
	else
		node->right = insertNode(node->right, key);

	return node;
}

/* 2分木の内容を表示する */
void printBtree(NODE *node)
{
	if (node == NULL)
		return;
	printBtree(node->left);
	printf("WORD: %s, COUNT: %d\n", node->key, node->count);
	printBtree(node->right);
}

/* 2分木を解放する */
void freeBtree(NODE *node)
{
	if (node == NULL)
		return;
	freeBtree(node->left);
	freeBtree(node->right);
	free(node->key);
	free(node);
}

int main(void)
{
	FILE *fp;
	char str[BUF_SIZE];

	NODE *node = NULL;

	if ((fp = fopen("Alice_in_Wonderland.txt", "r")) == NULL) {
		/* ファイルが開けない場合には終了する */
		fprintf(stderr, "ファイルを開けません。\n");
		return 1;
	}

	/* 1単語ずつ読み取り、2分木に追加する */
	while (fscanf(fp, "%s", str) != EOF) {
		node = insertNode(node, str);
	}

	/* 2分木の内容を表示する */
	printBtree(node);
	/* 2分木を解放する */
	freeBtree(node);

	fclose(fp);

	return 0;
}

