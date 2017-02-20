/*
 ============================================================================
 Name        : leaSearch.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 查找算法
 ============================================================================
 */

#ifndef LITEEA_LEASEARCH_H_
#define LITEEA_LEASEARCH_H_

#ifdef __cplusplus
extern "C" {
#endif
/**
 * 顺序查找
 */
int seqSearch(int D[], int n, int key);

/**
 * 二分查找
 */
int binSearch(int D[], int n, int key);

#ifdef __cplusPlus
}
#endif
#endif /* LITEEA_LEASEARCH_H_ */
