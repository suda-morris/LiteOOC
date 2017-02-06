/*
 ============================================================================
 Name        : leaSort.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 排序算法
 ============================================================================
 */
#include "leaSort.h"

/**
 * 插入排序(稳定)
 * @param D 待排序数组
 * @param n 数组长度
 * 时间复杂度：O(n^2)
 */
void insertSort(int D[], int n) {
	int i, j;
	int key;
	/* n-1趟排序 */
	for (i = 1; i < n; i++) {
		key = D[i];
		/* 挪位 */
		for (j = i - 1; j >= 0 && D[j] > key; j--) {
			D[j + 1] = D[j];
		}
		D[j + 1] = key;
	}
}

/**
 * 选择排序(不稳定)
 * @param D 待排序数组
 * @param n 数组长度
 * 时间复杂度：O(n^2)
 */
void selectionSort(int D[], int n) {
	int i, j;
	int max;
	for (i = 1; i < n; i++) {
		max = n - i;		//将max初始化为待排序元素的最末尾位置
		for (j = 0; j < n - i - 1; j++) {
			if (D[j] > D[max]) {	//寻找待排序元素中最大元素的位置
				max = j;
			}
		}
		if (max != n - i) {	//将最大元素交换到待排元素的末尾
			D[max] = D[max] ^ D[n - i];
			D[n - i] = D[max] ^ D[n - i];
			D[max] = D[max] ^ D[n - i];
		}
	}
}

/**
 * 冒泡排序(稳定)
 * @param D 待排序数组
 * @param n 数组长度
 * 时间复杂度：O(n^2)
 */
void bubbleSort(int D[], int n) {
	int changed = 0;
	int i = 1, j = 0;
	/* 最多n-1趟 */
	do {
		changed = 0;
		for (j = 0; j < n - i; j++) {
			if (D[j] > D[j + 1]) {
				/* 利用按位异或运算交换两数的值，省去中间变量 */
				D[j] = D[j] ^ D[j + 1];
				D[j + 1] = D[j] ^ D[j + 1];
				D[j] = D[j] ^ D[j + 1];
				changed = 1;	//本趟排序发生了元素交换
			}
		}
		i++;
	} while (i < n && changed == 1);
}

/* 序列划分函数 */
static int partition(int D[], int p, int r) {
	int i, j;
	i = p - 1;
	for (j = p; j < r; j++) {
		if (D[j] <= D[r]) {	//发现小于划界元素的键值
			i++;
			if (D[i] != D[j]) {	//使用异或来交换数值
				D[i] = D[i] ^ D[j];
				D[j] = D[i] ^ D[j];
				D[i] = D[i] ^ D[j];
			}
		}
	}
	if (D[i + 1] != D[r]) {
		D[i + 1] = D[i + 1] ^ D[r];
		D[r] = D[i + 1] ^ D[r];
		D[i + 1] = D[i + 1] ^ D[r];
	}
	return i + 1;
}

/**
 * 快速排序(不稳定)
 * @param D 待排序数组
 * @param p 起始元素下标，从0开始
 * @param r 末尾元素下标，从0开始
 * 时间复杂度：O(nlog2n)
 */
void quickSort(int D[], int p, int r) {
	int position = 0;
	if (p < r) {
		position = partition(D, p, r);	//返回划界元素最终位置
		quickSort(D, p, position - 1);	//对划分的子序列进行递归操作
		quickSort(D, position + 1, r);
	}
}

/* 合并函数 */
static void merge(int A[], int B[], int C[], int lengthA, int lengthB) {
	int a = 0, b = 0, c = 0;
	while (c < lengthA + lengthB) {
		if (A[a] < B[b]) {
			C[c++] = A[a++];
		} else {
			C[c++] = B[b++];
		}
		if (a == lengthA) {
			while (b < lengthB) {
				C[c++] = B[b++];
			}
		} else if (b == lengthB) {
			while (a < lengthA) {
				C[c++] = A[a++];
			}
		}
	}
}

/**
 * 归并排序(稳定)
 * @param A 待排序的数组
 * @param n 数组长度
 * 时间复杂度：O(nlog2n)
 */
void mergeSort(int A[], int n) {
	/* 将数组划分成子数组 */
	if (n > 1) {
		int i = n / 2;
		int j = n - n / 2;
		int B[n / 2];
		int C[n - n / 2];
		int k;
		for (k = 0; k < i; k++) {
			B[k] = A[k];
		}
		for (k = 0; k < j; k++) {
			C[k] = A[k + i];
		}
		/* 子数组递归进行归并排序 */
		mergeSort(B, i);
		mergeSort(C, j);
		/* 合并有序子序列 */
		merge(B, C, A, i, j);
	}
}

/**
 * 希尔排序(不稳定)
 * @param D 待排序数组
 * @param n 数组长度
 * 时间复杂度：O(nlog2n)
 */
void shellSort(int D[], int n) {
	int i, gap;
	int changed = 0;
	/* 采用shell增量法取步长 */
	for (gap = n / 2; gap > 0; gap /= 2) {
		/* 采用冒泡排序 */
		do {
			changed = 0;
			for (i = 0; i < n - gap; i++) {
				if (D[i] > D[i + gap]) {
					changed = 1;
					D[i] = D[i] ^ D[i + gap];
					D[i + gap] = D[i] ^ D[i + gap];
					D[i] = D[i] ^ D[i + gap];
				}
			}
		} while (changed == 1);
	}
}
