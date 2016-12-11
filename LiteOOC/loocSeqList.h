/*
 ============================================================================
 Name        : loocBinTree.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 顺序表
 ============================================================================
 */

#ifndef LITEOOC_LOOCSEQLIST_H_
#define LITEOOC_LOOCSEQLIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <looc.h>
#include "loocObject.h"

	/* 顺序表默认大小 */
#define LOOC_SEQLIST_DEFAULT_SIZE		(10)

	/**
	 * 顺序表抽象类
	 */
	ABS_CLASS(loocSeqList) {
		/* 继承自loocObject */
		EXTENDS(loocObject);
		/* 顺序表最大大小 */
		int maxSize;
		/* 顺序表表头 */
		void* head;
	};

#ifdef __cplusPlus
}
#endif

#endif /* LITEOOC_LOOCSEQLIST_H_ */
