/*
 * 学籍番号・氏名 : 163334 佐野和輝
 * 授業回・課題番号 : 第7回・第1課題
 * 工夫した点 : 構造体を活用することによって、ソースコードの可読性が高くなるようにした。また、マップの更新では、セルの状態が変化する場合のみ代入を行うようにすることで、高速化を行った。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 座標データ構造体(datファイルの読み出しで使う) */
typedef struct {
	int x;
	int y;
} Coordinate;

/* マップの大きさを読み取る */
void read_size(FILE *fp, int *width, int *height)
{
	fread(width, sizeof(*width), 1, fp);
	fread(height, sizeof(*height), 1, fp);
}

/* 座標データを読み取る */
Coordinate *read_coordinates(FILE *fp, int *n)
{
	*n = 0;
	int coordinate[2];
	int i;
	Coordinate *coordinates;

	/* 座標データ数を求める */
	while (fread(coordinate, sizeof(coordinate), 1, fp) == 1)
		(*n)++;

	/* 座標データの先頭にカーソルを合わせる */
	fseek(fp, sizeof(int) * 2, SEEK_SET);

	/* 座標データ配列用の領域を確保する */
	if ((coordinates = (Coordinate*)malloc(sizeof(Coordinate) * *n)) == NULL) {
		fprintf(stderr, "malloc() failed.\n");
		exit(1);
	}

	/* 座標データを読み取る */
	for (i = 0; i < *n; i++) {
		fread(coordinate, sizeof(coordinate), 1, fp);
		coordinates[i].x = coordinate[0];
		coordinates[i].y = coordinate[1];
	}

	return coordinates;
}

/* マップを生成する */
char **alloc_map(int width, int height)
{
	char **map;
	int i;

	/* マップ用の領域を確保する */
	if ((map = (char**)malloc(sizeof(char*) * height)) == NULL) {
		fprintf(stderr, "malloc() failed.\n");
		exit(1);
	}
	for (i = 0; i < height; i++) {
		if ((map[i] = (char*)malloc(sizeof(char) * width)) == NULL) {
			fprintf(stderr, "malloc() failed.\n");
			exit(1);
		}
	}

	return map;
}

/* マップを解放する */
void free_map(char **map, int height)
{
	int i;

	for (i = 0; i < height; i++)
		free(map[i]);
	free(map);
}

/* マップを座標データ配列を基に初期化する */
void init_map(char **map, int width, int height, Coordinate *coordinates, int n)
{
	int i;

	for (i = 0; i < height; i++) {
		int j;
		for (j = 0; j < width; j++)
			map[i][j] = 0;
	}
	if (coordinates == NULL)
		return;
	for (i = 0; i < n; i++) {
		int x, y;
		x = coordinates[i].x;
		y = coordinates[i].y;
		map[y][x] = 1;
	}
}

/* マップを表示する */
void display_map(FILE *pipe, char **map, int width, int height)
{
	int i;

	fprintf(pipe, "plot '-' with points pointtype 4\n");
	for (i = 0; i < height; i++) {
		int j;
		for (j = 0; j < width; j++) {
			if (!map[i][j])
				continue;
			fprintf(pipe, "%d %d\n", j, i);
		}
	}
	fprintf(pipe, "e\n");
}

/* (x,y)の周囲8マスにおいて、生きているセルの数を求める */
int count_live_cell(char **map, int width, int height, int x, int y)
{
	int i;
	int count = 0;
	/* (x,y)からの変位テーブル */
	int dtable[][2] = {
		{1, 0},
		{1, 1},
		{0, 1},
		{-1, 1},
		{-1, 0},
		{-1, -1},
		{0, -1},
		{1, -1},
	};

	for (i = 0; i < sizeof(dtable) / sizeof(dtable[0]); i++) {
		int dx, dy;
		int x2, y2;
		/* (x,y)からの変位 */
		dx = dtable[i][0];
		dy = dtable[i][1];
		/* (x,y)周囲の座標を求める */
		x2 = x + dx;
		y2 = y + dy;
		/* マップの上下と左右が繋がっているものとして扱う */
		x2 += width;
		x2 %= width;
		y2 += height;
		y2 %= height;
		/* セルが生きているとき、map[y2][x2] == 1である */
		count += map[y2][x2];
	}

	return count;
}

/* マップを更新する */
void update_map(char **map, int width, int height)
{
	static char **count_map = NULL;
	int i;

	/* 関数の初回呼び出し時に作業用のマップを確保する */
	if (count_map == NULL)
		count_map = alloc_map(width, height);

	/* 周囲の生きているセルをカウントする */
	for (i = 0; i < height; i++) {
		int j;
		for (j = 0; j < width; j++)
			count_map[i][j] = count_live_cell(map, width, height, j, i);
	}
	/* セルの状態を更新する */
	for (i = 0; i < height; i++) {
		int j;
		for (j = 0; j < width; j++) {
			int count = count_map[i][j];
			/* 状態が遷移する場合のみ扱う */
			if ((count == 3) && !map[i][j])
				map[i][j] = 1;
			else if ((count <= 1 || count >= 4) && map[i][j])
				map[i][j] = 0;
		}
	}
}

int main(int argc, char **argv)
{
	FILE *fp;
	int width, height;
	Coordinate *coordinates;
	int n;
	FILE *pipe;
	char **map;

	/* コマンドライン引数のチェック */
	if (argc != 2) {
		fprintf(stderr, "コマンドライン引数が正しくありません\n");
		return 1;
	}

	/* ファイルをオープンする */
	if ((fp = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "ファイルのオープンに失敗しました\n");
		return 1;
	}

	/* マップの大きさを読み取る */
	read_size(fp, &width, &height);

	/* 座標データ配列を読み取る */
	coordinates = read_coordinates(fp, &n);

	/* ファイルを閉じる */
	fclose(fp);

	/* マップを生成する */
	map = alloc_map(width, height);
	init_map(map, width, height, coordinates, n);
	/* 座標データ配列を解放する */
	free(coordinates);

	/* gnuplotを起動する */
	pipe = popen("gnuplot -persist", "w");
	fprintf(pipe, "set terminal xterm\n");
	fprintf(pipe, "set xrange [0:%d]\n", width);
	fprintf(pipe, "set yrange [0:%d]\n", height);

	/* マップの表示と更新を繰り返す */
	while (1) {
		display_map(pipe, map, width, height);
		update_map(map, width, height);
	}

	/* gnuplotへのパイプを閉じる */
	pclose(pipe);

	/* マップを解放する */
	free_map(map, height);

	return 0;
}

