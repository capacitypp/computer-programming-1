/*
 * 学籍番号・氏名 : 163334 佐野和輝
 * 授業回・課題番号 : 第4回・第2課題
 * 工夫した点 : リストを解放する関数と、ハッシュテーブルを解放する関数を実装し、実際に利用するようにした。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE	8191
#define BUF_SIZE	256

typedef struct list {
	char *key;
	int count;
	struct list *next;
} LIST;

typedef struct hash_table {
	LIST **table;
} HASH_TABLE;

/* Listを生成する */
LIST *allocList(char *key)
{
	LIST *ptr;

	if ((ptr = (LIST*)malloc(sizeof(LIST))) == NULL) {
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
	ptr->next = NULL;

	return ptr;
}

/* リストに挿入する */
LIST *insertList(LIST *list, char *key)
{
	/* 同じキーを持つリストが存在しない場合 */
	if (list == NULL) {
		/* Listを生成する */
		list = allocList(key);
		list->count++;
		return list;
	}
	int let = strcmp(list->key, key);
	/* 同じキーを持っていたらカウントを増やす
	 * そうでなければ再帰的に処理する */
	if  (!let)
		list->count++;
	else
		list->next = insertList(list->next, key);

	return list;
}

/* リストの内容を表示する */
void printList(LIST *list)
{
	if (list == NULL)
		return;
	printf("WORD: %s, COUNT: %d\n", list->key, list->count);
	printList(list->next);
}

/* リストを解放する */
void freeList(LIST *list)
{
	if (list == NULL)
		return;
	freeList(list->next);
	free(list->key);
	free(list);
}

/* ハッシュ関数 */
int hash(char *str)
{
	int h = 0, n;
	for (n = 0; str[n] != '\0'; n++)
		h = (h * 137 + str[n]) % HASHSIZE;
	return h;
}

/* ハッシュテーブルを生成する */
HASH_TABLE *allocHashTable()
{
	int i;

	HASH_TABLE *hash_table;

	if ((hash_table = (HASH_TABLE*)malloc(sizeof(HASH_TABLE))) == NULL) {
		fprintf(stderr, "malloc() failed");
		exit(1);
	}
	if ((hash_table->table = (LIST**)malloc(sizeof(LIST*) * HASHSIZE)) == NULL) {
		fprintf(stderr, "malloc() failed");
		exit(1);
	}
	for (i = 0; i < HASHSIZE; i++)
		hash_table->table[i] = NULL;

	return hash_table;
}

/* ハッシュテーブルに挿入する */
void insertHashTable(HASH_TABLE *hash_table, char *key)
{
	/* ハッシュテーブルにおける添え字を求める */
	int idx = hash(key);
	/* 添え字に対応するリストに挿入する */
	hash_table->table[idx] = insertList(hash_table->table[idx], key);
}

/* ハッシュテーブルの内容を表示する */
void printHashTable(HASH_TABLE *hash_table)
{
	int i;
	for (i = 0; i < HASHSIZE; i++)
		printList(hash_table->table[i]);
}

/* ハッシュテーブルを解放する */
void freeHashTable(HASH_TABLE *hash_table)
{
	int i;

	if (hash_table == NULL)
		return;
	for (i = 0; i < HASHSIZE; i++)
		freeList(hash_table->table[i]);
	free(hash_table->table);
	free(hash_table);
}

int main(void)
{
	FILE *fp;
	char str[BUF_SIZE];

	HASH_TABLE *hash_table = allocHashTable();

	if ((fp = fopen("Alice_in_Wonderland.txt", "r")) == NULL) {
		/* ファイルが開けない場合には終了する */
		fprintf(stderr, "ファイルを開けません。\n");
		return 1;
	}

	/* 1単語ずつ読み取り、リストに追加する */
	while (fscanf(fp, "%s", str) != EOF) {
		insertHashTable(hash_table, str);
	}

	/* リストの内容を表示する */
	printHashTable(hash_table);
	/* リストを解放する */
	freeHashTable(hash_table);

	fclose(fp);

	return 0;
}

