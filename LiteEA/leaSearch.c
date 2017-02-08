/*
 ============================================================================
 Name        : leaSearch.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 查找算法
 ============================================================================
 */
#include "leaSearch.h"

/**
 * 顺序查找
 * @param  D   带查找数组
 * @param  n   数组长度
 * @param  key 查找的关键字
 * @return     成功返回位置，失败返回-1
 * 时间复杂度：O(n)
 */
int seqSearch(int D[], int n, int key) {
	int i;
	for (i = 0; i < n; i++) {
		if (D[i] == key) {
			return i;
		}
	}
	return -1;
}

/**
 * 二分查找
 * @param  D   带查找数组
 * @param  n   数组长度
 * @param  key 查找的关键字
 * @return     成功返回位置，失败返回-1
 * 时间复杂度：O(log2n)
 */
int binSearch(int D[], int n, int key) {
	int low = 0, high = n - 1;
	int mid;
	while (low <= high) {
		mid = (low + high) / 2;
		if (key == D[mid]) {
			return mid;
		} else if (D[mid] > key) {
			high = mid - 1;
		} else {
			low = mid + 1;
		}
	}
	return -1; //查找失败
}
