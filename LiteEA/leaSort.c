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
#include <stdlib.h>

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
		for (j = 0; j < n - i; j++) {
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
static int partition(int D[], int low, int high) {
	int middle = low + (high - low) / 2;
	if (D[low] > D[high]) {
		D[low] = D[low] ^ D[high];
		D[high] = D[low] ^ D[high];
		D[low] = D[low] ^ D[high];
	}
	if (D[middle] > D[high]) {
		D[middle] = D[middle] ^ D[high];
		D[high] = D[middle] ^ D[high];
		D[middle] = D[middle] ^ D[high];
	}
	if (D[middle] > D[low]) {
		D[middle] = D[middle] ^ D[low];
		D[low] = D[middle] ^ D[low];
		D[middle] = D[middle] ^ D[low];
	}
	int point = D[low];
	while (low < high) {
		while (low < high && D[high] >= point) {
			high--;
		}
		D[low] = D[high];
		while (low < high && D[low] <= point) {
			low++;
		}
		D[high] = D[low];
	}
	D[low] = point;
	return low;
}

/**
 * 快速排序(不稳定)
 * @param D 待排序数组
 * @param low 起始元素下标，从0开始
 * @param high 末尾元素下标，从0开始
 * 时间复杂度：O(nlog2n)
 * 以完成四项优化：	1. 使用中间数充当枢轴，避免序列划分后得到的序列长度相差很大
 * 				2. 递归过程中当序列长度小于7的时候，选择使用插入排序
 * 				3. 减少不必要的数据交换，改为数据赋值
 * 				4. 使用尾递归，优化递归的性能
 */
void quickSort(int D[], int low, int high) {
	int point;
	if ((high - low) > 7) {
		while (low < high) {
			point = partition(D, low, high);
			if ((point - low) < (high - point)) {
				quickSort(D, low, point - 1);
				low = point + 1;
			} else {
				quickSort(D, point + 1, high);
				high = point - 1;
			}
		}
	} else {
		insertSort(D + low, high - low + 1);
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

static void keepHeap(int D[], int heap_size, int k) {
	int left = 2 * k + 1;
	int right = 2 * k + 2;
	int largest = k;
	/* 判断节点k与其左右孩子节点的大小 */
	if (heap_size > left && D[left] > D[largest]) {
		largest = left;
	}
	if (heap_size > right && D[right] > D[largest]) {
		largest = right;
	}
	/* 如果子节点更大则交换 */
	if (largest != k) {
		D[k] = D[k] ^ D[largest];
		D[largest] = D[k] ^ D[largest];
		D[k] = D[k] ^ D[largest];
		/* 递归，保持堆的性质 */
		keepHeap(D, heap_size, largest);
	}
}

static void buildHeap(int D[], int heap_size) {
	/* 最后一个非叶子节点 */
	int i = heap_size / 2 - 1;
	while (i >= 0) {
		/* 对每个节点修正堆的性质 */
		keepHeap(D, heap_size, i);
		i--;
	}
}

/**
 * 堆排序(不稳定)
 * @param D 待排序数组
 * @param n 数组长度
 * 时间复杂度：O(nlog2n)
 */
void heapSort(int D[], int n) {
	int i;
	int heap_size = n;
	buildHeap(D, heap_size);	//此时是一个无序堆
	/* 将无序堆中的根节点交换至无序堆的末尾 */
	for (i = heap_size - 1; i > 0; i--) {
		if (D[0] != D[i]) {
			D[0] = D[0] ^ D[i];
			D[i] = D[0] ^ D[i];
			D[0] = D[0] ^ D[i];
		}
		/* 无序堆的大小减一，整个数组的末尾是有序的 */
		heap_size--;
		/* 修正堆的根节点 */
		keepHeap(D, heap_size, 0);
	}
}

/**
 * 计数排序(稳定)
 * @param D 待排序数组
 * @param n 数组长度
 * 时间复杂度：O(n)
 */
void countingSort(int D[], int n) {
	int i, j;
	int k;
	int max = D[0], min = D[0];
	int *B;
	/* 求最大最小值 */
	for (i = 0; i < n; i++) {
		if (D[i] > max) {
			max = D[i];
		}
		if (D[i] < min) {
			min = D[i];
		}
	}
	k = max - min + 1;	//数组B的长度
	B = malloc(k * sizeof(int));
	for (i = 0; i < k; i++) {
		B[i] = 0;
	}
	/* 存放计数值 */
	for (i = 0; i < n; i++) {
		B[D[i] - min]++;
	}
	/* 反向填充目标数组 */
	j = 0;
	for (i = 0; i < k; i++) {
		while (B[i] > 0) {
			D[j++] = i + min;
			B[i]--;
		}
	}
	free(B);
}

/**
 * 基数排序(稳定)
 * @param D 待排序数组
 * @param n 数组长度
 * @param d 所有元素最多位数
 * @param w 基数，一般取10
 * 时间复杂度：O(d(n+w))
 */
void radixSort(int D[], int n, int d, int w) {
	int* temp = malloc(n * sizeof(int));	//存储每趟排序的中间数据
	int* count = malloc(w * sizeof(int));
	int i, j, k;
	int degree = 1;
	/* 总共排序d趟 */
	for (i = 0; i < d; i++) {
		/* 计数排序 */
		for (j = 0; j < w; j++) {
			count[j] = 0;
		}
		/* 统计每一位上相同值的元素个数 */
		for (j = 0; j < n; j++) {
			k = D[j] / degree % 10;
			count[k]++;
		}
		/* 计算在原数组中各组值的结束位置 */
		for (j = 1; j < w; j++) {
			count[j] += count[j - 1];
		}
		/* 从后往前依次将各组的值放入临时数组中，这样才能不破坏稳定性 */
		for (j = n - 1; j >= 0; j--) {
			k = D[j] / degree % 10;
			count[k]--;
			temp[count[k]] = D[j];
		}
		for (j = 0; j < n; j++) {
			D[j] = temp[j];
		}
		degree *= 10;
	}
	free(temp);
	free(count);
}
