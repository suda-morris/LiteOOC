/*
 ============================================================================
 Name        : leaSort.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 排序算法
 排序算法的选择：
 1、 在通常情况下，即输入数据是随机的，快速排序、归并排序、希尔排序和堆排序的运行速度较快，其中堆排序是原地排序最节省空间，
 快速排序的速度是最快的。所以如果在内存空间不紧张的情况下，一般采用快速排序，如果需要节省空间则采用堆排序。
 2、 希尔排序不适合用在链表数据结构上
 3、 如果待排序数据规模不大且一开始就局部有序，插入排序和冒泡排序的运行时间最快，一般选择这两种排序算法
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

/**
 * 堆排序
 */
void heapSort(int D[], int n);

/**
 * 计数排序
 */
void countingSort(int D[], int n);

/**
 * 基数排序
 */
void radixSort(int D[], int n, int d, int w);

#ifdef __cplusPlus
}
#endif

#endif /* LITEEA_LEASORT_H_ */
