/*
 ============================================================================
 Name        : loocMatrix.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 矩阵
 ============================================================================
 */
#include "loocMatrix.h"
#include <stdio.h>

/**
 * 矩阵初始化
 * @param cthis 当前矩阵对象指针
 * @param row   矩阵行数
 * @param col   矩阵列数
 */
static void loocMatrix_init(loocMatrix* cthis, int row, int col) {
	int i, j;
	cthis->cols = col;
	cthis->rows = row;
	cthis->matrix_pool = (double**) looc_malloc(sizeof(double*) * row,
			"loocMatrix_pool_row",
			looc_file_line);
	for (i = 0; i < row; i++) {
		cthis->matrix_pool[i] = (double*) looc_malloc(sizeof(double) * col,
				"loocMatrix_pool_col", looc_file_line);
	}
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			cthis->matrix_pool[i][j] = 0;
		}
	}
}

/**
 * 矩阵转置
 * @param  cthis 当前矩阵对象指针
 * @return       返回转置后的新的矩阵
 */
static loocMatrix* loocMatrix_transpose(loocMatrix* cthis) {
	int i, j;
	loocMatrix* t = loocMatrix_new(looc_file_line);
	t->init(t, cthis->cols, cthis->rows);
	for (i = 0; i < t->rows; i++) {
		for (j = 0; j < t->cols; j++) {
			t->matrix_pool[i][j] = cthis->matrix_pool[j][i];
		}
	}
	return t;
}

/**
 * 矩阵打印函数
 * @param cthis 当前矩阵对象指针
 */
static void loocMatrix_print(loocMatrix* cthis) {
	int i, j;
	for (i = 0; i < cthis->rows; i++) {
		for (j = 0; j < cthis->cols; j++) {
			printf("%lf\t", cthis->matrix_pool[i][j]);
		}
		printf("\r\n");
	}
}

/**
 * 矩阵销毁函数
 * @param object loocObject对象指针
 */
static void loocMatrix_finalize(loocObject* object) {
	int i;
	loocMatrix* matrix = SUB_PTR(object, loocObject, loocMatrix);
	if (matrix->matrix_pool) {
		for (i = 0; i < matrix->rows; i++) {
			looc_free(matrix->matrix_pool[i]);
		}
		looc_free(matrix->matrix_pool);
	}
	matrix->cols = 0;
	matrix->rows = 0;
}

/**
 * loocMatrix的构造函数
 */
CTOR(loocMatrix)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->rows = 0;
	cthis->cols = 0;
	cthis->matrix_pool = NULL;
	/* 成员函数的绑定 */
	FUNCTION_SETTING(init, loocMatrix_init);
	FUNCTION_SETTING(transpose, loocMatrix_transpose);
	FUNCTION_SETTING(print, loocMatrix_print);
	FUNCTION_SETTING(loocObject.finalize, loocMatrix_finalize);END_CTOR

/**
 * loocMatrix的析构函数
 */
DTOR(loocMatrix)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR

/**
 * 矩阵加法
 * @param  A 矩阵A
 * @param  B 矩阵B
 * @return   返回A+B后的新矩阵
 */
loocMatrix* loocMatrix_addition(loocMatrix* A, loocMatrix* B) {
	int i, j;
	loocMatrix* C = NULL;
	/* 判断A和B的规格是否一样 */
	if (A->cols != B->cols || A->rows != B->rows) {
		return NULL;
	}
	C = loocMatrix_new(looc_file_line);
	C->init(C, A->rows, A->cols);
	for (i = 0; i < A->rows; i++) {
		for (j = 0; j < A->cols; j++) {
			C->matrix_pool[i][j] = A->matrix_pool[i][j] + B->matrix_pool[i][j];
		}
	}
	return C;
}

/**
 * 矩阵减法
 * @param  A 矩阵A
 * @param  B 矩阵B
 * @return   返回A-B后的新的矩阵
 */
loocMatrix* loocMatrix_subtraction(loocMatrix* A, loocMatrix* B) {
	int i, j;
	loocMatrix* C = NULL;
	/* 判断A和B的规格是否一样 */
	if (A->cols != B->cols || A->rows != B->rows) {
		return NULL;
	}
	C = loocMatrix_new(looc_file_line);
	C->init(C, A->rows, A->cols);
	for (i = 0; i < A->rows; i++) {
		for (j = 0; j < A->cols; j++) {
			C->matrix_pool[i][j] = A->matrix_pool[i][j] - B->matrix_pool[i][j];
		}
	}
	return C;
}

/**
 * 矩阵乘法
 * @param  A 矩阵A
 * @param  B 矩阵B
 * @return   返回A*B后的新矩阵
 */
loocMatrix* loocMatrix_multiplication(loocMatrix* A, loocMatrix* B) {
	int i, j, k;
	loocMatrix* C = NULL;
	/* 判断A的列数是否等于B的行数 */
	if (A->cols != B->rows) {
		return NULL;
	}
	C = loocMatrix_new(looc_file_line);
	C->init(C, A->rows, A->cols);
	for (i = 0; i < A->rows; i++) {
		for (j = 0; j < B->cols; j++) {
			for (k = 0; k < A->cols; k++) {
				C->matrix_pool[i][j] += A->matrix_pool[i][k]
						* B->matrix_pool[k][j];
			}
		}
	}
	return C;
}
