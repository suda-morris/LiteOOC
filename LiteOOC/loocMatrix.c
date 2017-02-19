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

/**
 * 创建一个n阶的幻方
 * @param  n 幻方的阶数
 * @return   返回创建后的幻方
 */
loocMatrix* loocMatrix_magicMatrix(int n) {
	int row = 0;
	int col = n / 2;
	int i;
	loocMatrix* magic;
	if (n <= 0 || n % 2 == 0) {
		return NULL;
	}
	magic = loocMatrix_new(looc_file_line);
	magic->init(magic, n, n);
	/* 初始化第一行中间为1 */
	magic->matrix_pool[row][col] = 1;
	/* 斜排法往幻方中填数2~n^2 */
	for (i = 2; i <= n * n; i++) {
		/* 右上，行没有越界 */
		if (row - 1 >= 0) {
			/* 右上，列没有越界 */
			if (col + 1 < n) {
				if (magic->matrix_pool[row - 1][col + 1] == 0) {
					magic->matrix_pool[row - 1][col + 1] = i;
					row = row - 1;
					col = col + 1;
				}
				/* 右上已有数据 */
				else {
					magic->matrix_pool[row + 1][col] = i;
					row = row + 1;
				}
			}
			/* 右上，列越界 */
			else {
				if (magic->matrix_pool[row - 1][0] == 0) {
					magic->matrix_pool[row - 1][0] = i;
					row = row - 1;
					col = 0;
				}
				/* 右上已有数据 */
				else {
					magic->matrix_pool[row + 1][col] = i;
					row = row + 1;
				}
			}
		}
		/* 右上，行越界 */
		else {
			/* 右上，列没有越界 */
			if (col + 1 < n) {
				if (magic->matrix_pool[n - 1][col + 1] == 0) {
					magic->matrix_pool[n - 1][col + 1] = i;
					row = n - 1;
					col = col + 1;
				}
				/* 右上已有数据 */
				else {
					magic->matrix_pool[row + 1][col] = i;
					row = row + 1;
				}
			}
			/* 右上，列越界 */
			else {
				if (magic->matrix_pool[n - 1][0] == 0) {
					magic->matrix_pool[n - 1][0] = i;
					row = n - 1;
					col = 0;
				}
				/* 右上已有数据 */
				else {
					magic->matrix_pool[row + 1][col] = i;
					row = row + 1;
				}
			}
		}
	}
	return magic;
}

/********************************N皇后问题***************************/
static int NQueueCount = 0;

/**
 * 判断位置(row,col)是否适合放棋子
 */
static int NQueue_isSafe(int row, int col, loocMatrix* chess) {
	int i, j;
	int n = chess->rows;
	int flagRow = 0, flagCol = 0, flagLeftUp = 0, flagLeftDown = 0,
			flagRightUp = 0, flagRightDown = 0;
	for (i = 0; i < n; i++) {
		if (chess->matrix_pool[i][col]) {
			flagCol = 1;
			break;
		}
		if (chess->matrix_pool[row][i]) {
			flagRow = 1;
			break;
		}
	}
	for (i = row, j = col; i >= 0 && j >= 0; i--, j--) {
		if (chess->matrix_pool[i][j]) {
			flagLeftUp = 1;
			break;
		}
	}
	for (i = row, j = col; i < n && j >= 0; i++, j--) {
		if (chess->matrix_pool[i][j]) {
			flagLeftDown = 1;
			break;
		}
	}
	for (i = row, j = col; i >= 0 && j < n; i--, j++) {
		if (chess->matrix_pool[i][j]) {
			flagRightUp = 1;
			break;
		}
	}
	for (i = row, j = col; i < n && j < n; i++, j++) {
		if (chess->matrix_pool[i][j]) {
			flagRightDown = 1;
			break;
		}
	}
	if (flagRow || flagCol || flagLeftUp || flagLeftDown || flagRightUp
			|| flagRightDown) {
		return 0;
	}
	return 1;
}

static void _NQueen(int row, int show, loocMatrix* chess) {
	int n = chess->cols;
	int i, j, k;

	if (row == n) {
		/* 如果row=n，说明该棋盘布局完成 */
		NQueueCount++;	//增加方案数
		if (show) {
			printf("方案%d:\r\n", NQueueCount);
			chess->print(chess);	//打印棋盘
		}
	} else {
		for (k = 0; k < n; k++) {
			/* 避免前面递归过程中修改矩阵带来的干扰 */
			for (i = row; i < n; i++) {
				for (j = 0; j < n; j++) {
					chess->matrix_pool[i][j] = 0;
				}
			}
			/* 如果适合放置棋子，则递归进行下一行的检测 */
			if (NQueue_isSafe(row, k, chess)) {
				chess->matrix_pool[row][k] = 1;
				_NQueen(row + 1, show, chess);	//递归调用本函数
			}
		}
	}
}

/**
 * N皇后问题
 * @param  n    皇后的数量
 * @param  show 是否要打印出每个结果
 * @return      返回N皇后问题解的数量
 */
int loocMatrix_placeNQueen(int n, int show) {
	NQueueCount = 0;
	loocMatrix* chess = loocMatrix_new(looc_file_line);
	chess->init(chess, n, n);
	_NQueen(0, show, chess); //调用递归函数
	loocMatrix_delete(chess);
	return NQueueCount;
}
