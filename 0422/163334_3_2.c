/*
 * 学籍番号・氏名 : 163334 佐野和輝
 * 授業回・課題番号 : 第3回・第2課題
 * 工夫した点 : integral関数におけるx_leftとx_rightの更新は、本来stepを加えることでも行えるが、誤差が累積することを考慮して毎回iの値を元に計算するようにした。
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// target function 1
double f1(double x)
{
  return x * x;
}

// target function 2
double f2(double x)
{
  return x * x * x + x * x;
}

// target function 3
double f3(double x)
{
  return sin(x) + cos(x);
}

// calculation of the area of micro-trapezoid 
double trapezoid(double w1, double w2, double h)
{
  return (w1 + w2) * h / 2.0;
}

// general intergral function by calculating and summing up the area of micro-trapezoid
double integral(double x1, double x2, int n, double (*func)(double)) 
{
  double area = 0;
  double x_left, x_right;
  int i;
  double x1x2 = x2 - x1;
  double step = x1x2 / n;

  for (i = 0; i < n; i++) {
	/* 誤差が累積しないように、x_leftとx_rightは毎回計算する */
	x_left = x1x2 * i / n + x1;
	x_right = x1x2 * (i + 1) / n + x1;
	area += trapezoid(func(x_left), func(x_right), step);
  }

  return area;
}


int main()
{
  double (*func[])(double)= { f1, f2, f3 };
  double val[3][2] = { {1.0, 10.0}, {1.0, 10.0}, {M_PI / 3.0, M_PI} };

  int i;

  for (i = 0; i < 3; i++) {
    printf("f%d: %f\n", i, integral(val[i][0], val[i][1], 10000, func[i]));
  }

  return EXIT_SUCCESS;
}

/*
実行結果:

$ gcc -o 163334_3_2 163334_3_2.c 
$ ./163334_3_2
f0: 333.000001
f1: 2832.750021
f2: 0.633975

*/

