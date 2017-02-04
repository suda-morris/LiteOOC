/*
 ============================================================================
 Name        : lea.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 一些工程算法或面试题目
 ============================================================================
 */
#include "lea.h"

/**
 * 验证一个数组是否是一个二叉查找树的后序遍历结果
 * @param  squence 待验证的数组
 * @param  length  数组长度
 * @return         验证通过返回looc_true，否则返回looc_false
 */
looc_bool verifySquenceOfBST(int squence[], int length) {
	if (squence == NULL || length <= 0) {
		return looc_false;
	}
	/* 二叉查找树的根节点是后序遍历的最后一个元素 */
	int root = squence[length - 1];
	/* 左子树的节点小于根节点 */
	int i = 0;
	for (; i < length - 1; i++) {
		if (squence[i] > root) {
			break;
		}
	}
	/* 右子树的元素应该大于根节点 */
	int j = i;
	for (; j < length - 1; j++) {
		if (squence[j] < root) {
			return looc_false;
		}
	}
	/* 递归验证左子树是不是一个BST */
	looc_bool left = looc_true;
	if (i > 0) {
		left = verifySquenceOfBST(squence, i);
	}
	/* 递归验证右子树是不是一个BST */
	looc_bool right = looc_true;
	if (i < length - 1) {
		right = verifySquenceOfBST(squence + i, length - i - 1);
	}
	return (left & right);
}

static void _transNum(int origin, int num, int d, char result[]) {
	static int noEnd = 1;
	static int i = 0;
	int mod;

	if (num == origin) {
		i = 0;
		noEnd = 1;
	}

	mod = num % d;
	num = num / d;
	while (noEnd && num) {
		_transNum(origin, num, d, result);
	}
	noEnd = 0;
	if (mod >= 10) {
		mod = mod - 10 + 'A';
	} else {
		mod = mod + '0';
	}
	result[i++] = mod;
	result[i] = '\0';
}

/**
 * 数制转换(辗转相除)
 * @param num    待转换的数字
 * @param d      进制
 * @param result 存放转换后的字符串
 */
void transNum(int num, int d, char result[]) {
	_transNum(num, num, d, result);
}

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
