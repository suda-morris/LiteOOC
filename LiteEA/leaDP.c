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
	while (path[i] != -1) {
		printf("%d\t", x[i]);
		i = path[i];
	}
	printf("%d\r\n", x[i]);
}
