#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATANUM (100)

extern void bubblesort(void *, size_t, size_t, int (*)(const void *, const void *));

struct _DATAS {
  int key;
  double data;
} datas1[DATANUM], datas2[DATANUM];

typedef struct _DATAS DATAS;

void print_data1(double *data, int n)
{
  int i;

  for (i = 0; i < n; i++) {
    printf("DATA %2d: data=%f\n", i, data[i]);
  }
  printf("\n");
}

void print_data2(DATAS *data, int n)
{
  int i;

  for (i = 0; i < n; i++) {
    printf("DATA %2d: key=%3d, data=%f\n", i, data[i].key, data[i].data);
  }
  printf("\n");
}

int comp1(double *d1, double *d2)
{
#ifdef DEBUG
  printf("Now comparing %f and %f...\n", *d1, *d2);
#endif

  if (*d1 > *d2) return 1;   // return positive number if *d1 > *d2
  if (*d1 < *d2) return -1;  // return negative number if *d1 < *d2
  return 0;                  // return zero if *d1 == *d2
}

int comp2(DATAS *data1, DATAS *data2)
{
#ifdef DEBUG
  printf("Now comparing %d and %d...\n", data1->key, data2->key);
#endif

  return data1->key - data2->key; // return the difference between the key values
}

int main()
{
  int i;
  double dd1[DATANUM], dd2[DATANUM];
  clock_t t1, t2;

  srand(0);

  /*
   * part 1
   */
  printf("----- part 1-----\n");

  for (i = 0; i < DATANUM; i++) {
    dd1[i] = dd2[i] = (double)rand() / RAND_MAX * 1000.0;
  }

  printf("----- bubble sort -----\n");
  print_data1(dd1, DATANUM);
  t1 = clock();
  bubblesort(dd1, DATANUM, sizeof(double), (int(*)(const void*, const void*))comp1);
  t2 = clock();
  print_data1(dd1, DATANUM);
  printf("used CPU time: %.16f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

  printf("----- quick sort -----\n");
  print_data1(dd2, DATANUM);
  t1 = clock();
  qsort(dd2, DATANUM, sizeof(double), (int(*)(const void*, const void*))comp1);
  t2 = clock();
  print_data1(dd2, DATANUM);
  printf("used CPU time: %.16f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

  /*
   * part 2
   */
  printf("\n----- part 2-----\n");

  for (i = 0; i < DATANUM; i++) {
    datas1[i].key  = datas2[i].key  = (int)((double)rand() / RAND_MAX * 1000.0);
    datas1[i].data = datas2[i].data = (double)rand() / RAND_MAX * 1000.0;
  }

  printf("----- bubble sort -----\n");
  print_data2(datas1, DATANUM);
  t1 = clock();
  bubblesort(datas1, DATANUM, sizeof(DATAS), (int(*)(const void*, const void*))comp2);
  t2 = clock();
  print_data2(datas1, DATANUM);
  printf("used CPU time: %.16f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

  printf("----- quick sort -----\n");
  print_data2(datas2, DATANUM);
  t1 = clock();
  qsort(datas2, DATANUM, sizeof(DATAS), (int(*)(const void*, const void*))comp2);
  t2 = clock();
  print_data2(datas2, DATANUM);
  printf("used CPU time: %.16f\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

  return EXIT_SUCCESS;
}
