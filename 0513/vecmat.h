#ifndef ___Header_VECMAT
#define ___Header_VECMAT

typedef struct vector {
	int n;
	double *v;
} Vector;

typedef struct matrix {
	int n, m;
	double **t;
} Matrix;

Vector *new_vector(int n);
void free_vector(Vector *vector);
void vecprint(Vector *vector);

Matrix *new_matrix(int n, int m);
void free_matrix(Matrix *matrix);
void matprint(Matrix *matrix);

#endif

