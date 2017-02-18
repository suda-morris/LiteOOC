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
#include <stdio.h>

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
 * 判断CPU是否是小端模式
 * @return  如果是小端模式返回true，否则返回false
 */
looc_bool isLittleEndian(void) {
	union w {
		int a;
		char b;
	} c;
	c.a = 1;
	if (c.b == 1) {
		return looc_true;
	}
	return looc_false;
}

/**
 * 判断是否为完全平方数
 * @param  x 待判断的整数
 * @return   是完全平方数返回true，否则返回false
 */
looc_bool isPerfectSquare(int x) {
	int sum = 0;
	int i = 1;
	for (i = 1; sum < x; i += 2) {
		sum += i;
	}
	if (sum == x) {
		return looc_true;
	} else {
		return looc_false;
	}
}

/**
 * 计算第n个斐波那契数
 * @param  n 第几个斐波那契数
 * @return   返回计算结果
 */
int Fibonacci(int n) {
	int result;
	int last, nextToLast;
	int i;
	if (n <= 1) {
		return 1;
	}
	last = nextToLast = 1;
	for (i = 0; i < n - 1; i++) {
		result = last + nextToLast;
		nextToLast = last;
		last = result;
	}
	return result;
}

/**
 * RabinKarp字符串匹配算法
 * @param  T        待查字符串
 * @param  n        字符串T的长度
 * @param  P        模式字符串
 * @param  m        模式字符串长度
 * @param  q        将字符映射成数字用的模
 * @param  position 保存所有的有效位移
 * @return          返回有效位移的数量
 * 时间复杂度：O(m+n)
 */
int RabinKarp(char* T, int n, char* P, int m, int q, int* position) {
	int count = 0;
	int i, j;
	int h = 1;	//P元素最高次幂
	int p = 0;	//P的十进制值
	int t = 0;
	/* 求h的hash值 */
	for (i = 1; i < m; i++) {
		h = (h * 10) % q;
	}
	/* 计算模式字符串的hash值和待查字符串前m个字符的hash值 */
	for (i = 0; i < m; i++) {
		p = (10 * p + P[i]) % q;
		t = (10 * t + T[i]) % q;
	}
	/* 比较n-m+1次 */
	for (i = 0; i <= n - m; i++) {
		if (p == t) {
			/* 如果模值相等则使用朴素算法再次判断 */
			for (j = 0; j < m; j++) {
				if (P[j] != T[i + j]) {
					break;
				}
			}
			if (j >= m) {
				/* 保存有效位移 */
				position[count++] = i;
			}
		}
		if (i < n - m) {
			t = (10 * (t - T[i] * h) + T[i + m]) % q;	//递归求新的t值
			if (t < 0) {
				t = t + q;
			}
		}
	}
	return count;
}

/**
 * 最大公约数(辗转求余)
 * @param  a 整数a
 * @param  b 整数b
 * @return   返回a和b的最大公约数
 */
int gcd(int a, int b) {
	int max = a > b ? a : b;
	int min = max == a ? b : a;
	int r;
	while (min) {
		r = max % min;
		max = min;
		min = r;
	}
	return max;
}

/**
 * 判断一个数是否为素数
 * @param  number 待判定的数
 * @return        是素数就返回true，否则返回false
 */
looc_bool isPrime(int number) {
	int i;
	for (i = 2; i * i <= number; i++) {
		if (number % i == 0) {
			break;
		}
	}
	if (i * i > number) {
		return looc_true;
	} else {
		return looc_false;
	}
}

/**
 * 寻找指定范围内的所有素数
 * @param number 指定范围
 * @param result 保存指定范围内的素数
 */
void prime(int number, int* result) {
	int i, j;
	/* 初始化result数组保存1~number所有整数 */
	for (i = 0; i < number; i++) {
		result[i] = i + 1;
	}
	/* 筛选法找素数 */
	for (i = 2; i * i <= number; i++) {
		for (j = 2 * i; j <= number; j++) {
			if (j % i == 0) {
				result[j - 1] = 0;
			}
		}
	}
}
