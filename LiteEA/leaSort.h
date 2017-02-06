/*
 ============================================================================
 Name        : leaSort.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 排序算法
 ============================================================================
 */

#ifndef LITEEA_LEASORT_H_
#define LITEEA_LEASORT_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 插入排序
 */
void insertSort(int D[], int n);

/**
 * 选择排序
 */
void selectionSort(int D[], int n);

/**
 * 冒泡排序
 */
void bubbleSort(int D[], int n);

/**
 * 快速排序
 */
void quickSort(int D[], int p, int r);

/**
 * 归并排序
 */
void mergeSort(int A[], int n);

/**
 * 希尔排序
 */
void shellSort(int D[], int n);

#ifdef __cplusPlus
}
#endif

#endif /* LITEEA_LEASORT_H_ */
