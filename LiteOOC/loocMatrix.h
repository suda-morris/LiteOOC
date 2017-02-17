/*
 ============================================================================
 Name        : loocMatrix.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 矩阵
 ============================================================================
 */

#ifndef LITEOOC_LOOCMATRIX_H_
#define LITEOOC_LOOCMATRIX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "looc.h"
#include "loocObject.h"

	/**
	 * 矩阵的具体类
	 */
	CLASS(loocMatrix) {
		/* 继承自loocObject */
		EXTENDS(loocObject);
		/* 矩阵的行数 */
		int rows;
		/* 矩阵的列数 */
		int cols;
		/* 内存池 */
		double** matrix_pool;
		/* 初始化一个矩阵*/
		void (*init)(loocMatrix* cthis, int row, int col);
		/* 打印一个矩阵 */
		void (*print)(loocMatrix* cthis);
		/* 矩阵转置 */
		loocMatrix* (*transpose)(loocMatrix* cthis);
	};

	/* 矩阵加法 */
	loocMatrix* loocMatrix_addition(loocMatrix* A, loocMatrix* B);
	/* 矩阵减法 */
	loocMatrix* loocMatrix_subtraction(loocMatrix* A, loocMatrix* B);
	/* 矩阵乘法 */
	loocMatrix* loocMatrix_multiplication(loocMatrix* A, loocMatrix* B);

#ifdef __cplusPlus
}
#endif
#endif /* LITEOOC_LOOCMATRIX_H_ */
