/*
 ============================================================================
 Name        : leaDP.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 动态规划类题目
 ============================================================================
 */

#include "leaDP.h"
#include "looc.h"
#include <stdio.h>

/**
 * 最长公共子序列/串问题
 * @param x         x序列
 * @param y         y序列
 * @param m         x序列的长度
 * @param n         y序列的长度
 * @param result    LCS长度结果表
 * @param path      LCS路径表
 * @param continous 是否要求是串
 * @return        	返回最长LCS的值
 */
int LCS(int* x, int* y, int m, int n, int** result, int** path, int continous) {
	int i, j;
	int max = 0;
	/* 初始化 */
	for (i = 0; i <= m; i++) {
		for (j = 0; j <= n; j++) {
			result[i][j] = 0;
			path[i][j] = -1;
		}
	}
	/* 递归填表 */
	for (i = 1; i <= m; i++) {
		for (j = 1; j <= n; j++) {
			if (x[i - 1] == y[j - 1]) {
				result[i][j] = result[i - 1][j - 1] + 1;
				path[i][j] = 'i';	//‘i’表示前一个元素需要往左上搜索
				if (result[i][j] > max) {
					max = result[i][j];
				}
			}
			/* 求最长公共子串 */
			else if (continous) {
				result[i][j] = 0;
			}
			/* 求最长公共子序列 */
			else {
				if (result[i][j - 1] <= result[i - 1][j]) {
					result[i][j] = result[i - 1][j];
					path[i][j] = 'l';	//‘l’表示前一个元素需要往左搜索
				} else {
					result[i][j] = result[i][j - 1];
					path[i][j] = 'u';	//‘u’表示前一个元素需要往上搜索
				}
			}
		}
	}
	return max;
}

/**
 * 打印LCS表
 * @param  x      x序列
 * @param  y      y序列
 * @param  m      x序列长度
 * @param  n      y序列长度
 * @param  result LCS长度结果表
 * @param  path   LCS路径表
 * @param  max	  LCS最大值
 */
void printLCS(int* x, int* y, int m, int n, int** result, int** path, int max) {
	int i, j;
	printf("LCS_Res:\r\n");
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			printf("%d\t", result[i][j]);
		}
		printf("\r\n");
	}
	printf("LCS_Path:\r\n");
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			if (path[i][j] != -1) {
				printf("%c\t", path[i][j]);
			} else {
				printf("-\t");
			}
		}
		printf("\r\n");
	}
	for (i = m; i >= 0; i--) {
		for (j = n; j >= 0; j--) {
			if (result[i][j] == max) {
				break;
			}
		}
		if (j >= 0) {
			break;
		}
	}
	printf("LCS:\r\n");
	while (path[i][j] != -1) {
		if (path[i][j] == 'i') {
			printf("%d\t", x[i - 1]);
			i--;
			j--;
		} else if (path[i][j] == 'u') {
			j--;
		} else if (path[i][j] == 'l') {
			i--;
		}
	}
	printf("\r\n");
}

/**
 * 最长递增子序列问题
 * @param x      输入的序列
 * @param n      序列长度
 * @param result 各个子问题的结果(即序列前i个元素中的最长子序列长度)
 * @param path   最长子序列的必经路径
 * @return 		 返回LIS最大值
 */
int LIS(int* x, int n, int* result, int* path) {
	int i, j;
	int max = 0;
	/* 初始化 */
	for (i = 0; i < n; i++) {
		result[i] = 1;
		path[i] = -1;
	}
	/* 递归填表 */
	for (i = 1; i < n; i++) {
		for (j = 0; j < i; j++) {
			/* x[i]比x[j]要大，并且有助于LIS的增大 */
			if (x[i] > x[j] && result[i] < result[j] + 1) {
				result[i] = result[j] + 1;
				path[i] = j;	//保存前一个元素的下标
				if (result[i] > max) {
					max = result[i];
				}
			}
		}
	}
	return max;
}

/**
 * 打印LIS
 * @param x      x序列
 * @param n      x序列长度
 * @param result LIS长度表
 * @param path   LIS路径表
 * @param max    LIS最大值
 */
void printLIS(int* x, int n, int* result, int* path, int max) {
	int i;
	for (i = 0; i < n; i++) {
		if (result[i] == max) {
			break;
		}
	}
	printf("LIS:\r\n");
	while (path[i] != -1) {
		printf("%d\t", x[i]);
		i = path[i];
	}
	printf("%d\r\n", x[i]);
}

/**
 * 背包问题
 * @param  count    物品个数
 * @param  capacity 背包载重量
 * @param  weight   物品重量数组
 * @param  value    物品价值数组
 * @param  check    物品是否被选中数组，若果物品被装入，则为1
 * @return          返回背包能够装入最大的价值
 */
int Knapsack(int count, int capacity, int* weight, int* price, int* check) {
	int row = count + 1;
	int col = capacity + 1;
	int i, j;
	int max = 0;
	/* table表格保存子问题的解：即前i个物品能够装入载重量为j的背包中物品的最大价值 */
	int** value = (int**) looc_malloc(row * sizeof(int*), "Knapsack_table",
	looc_file_line);
	for (i = 0; i < row; i++) {
		value[i] = (int*) looc_malloc(col * sizeof(int), "Knapsack_table_i",
		looc_file_line);
	}
	/* 初始化所有物品都没有选中 */
	for (i = 0; i < count; i++) {
		check[i] = 0;
	}
	/* 初始化table表格 */
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			value[i][j] = 0;
		}
	}

	/* 递归填表 */
	for (i = 1; i < row; i++) {
		for (j = 1; j < col; j++) {
			/* 第i个物品先不装入背包 */
			value[i][j] = value[i - 1][j];
			/* 第i个物品的重量不超过背包的容量j */
			if (weight[i] <= j) {
				int temp = value[i - 1][j - weight[i]] + price[i];
				/* 装入的第i个物品有助于总价值的提升 */
				if (value[i][j] < temp) {
					value[i][j] = temp;
				}
			}
		}
	}
	/* 逆推求装入的物品 */
	j = capacity;
	for (i = count; i > 0; i--) {
		if (value[i][j] > value[i - 1][j]) {
			check[i] = 1;
			j -= weight[i];
		}
	}

	/* 记录最大价值 */
	max = value[row - 1][col - 1];
	/* 释放table的内存空间 */
	for (i = 0; i < row; i++) {
		looc_free(value[i]);
	}
	looc_free(value);
	return max;
}
