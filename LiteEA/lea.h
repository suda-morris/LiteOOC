/*
 ============================================================================
 Name        : lea.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 一些工程算法或面试题目
 ============================================================================
 */

#ifndef LITEEA_LEA_H_
#define LITEEA_LEA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "looc.h"
#include "leaSort.h"
#include "leaSearch.h"
#include "leaDP.h"

/**
 * 验证一个数组是否是一个二叉查找树的后序遍历的结果
 */
looc_bool verifySquenceOfBST(int squence[], int length);

/**
 * 数制转换，直接转换成字符串
 */
void transNum(int num, int d, char result[]);

/**
 * 判断CPU是否是小端模式
 */
looc_bool isLittleEndian(void);

/**
 * 判断整数x是否为完全平方数
 */
looc_bool isPerfectSquare(int x);

/**
 * 计算斐波那契数
 */
int Fibonacci(int n);

/**
 * 字符串匹配算法
 */
int RabinKarp(char* T, int n, char* P, int m, int q, int* position);

/**
 * 最大公约数
 */
int gcd(int a, int b);

/**
 * 判断一个数是否为素数
 */
looc_bool isPrime(int number);
/**
 * 寻找指定范围内的所有素数
 */
void prime(int number, int* result);

/**
 * 将n张牌重新洗牌
 */
void shuffle(int* card, int n);

/**
 * 约瑟夫问题
 */
void Josephus(int n, int m, int* order);

/**
 * 汉诺塔问题
 */
int stepsOfHanoi(int n, char a, char b, char c, int show);

/**
 * 迭代法求平方根
 */
double SQR(double a);

/**
 * 一重定积分
 */
double DefiniteIntegration(double (*express)(double x), double down, double up);

#ifdef __cplusPlus
}
#endif

#endif /* LITEEA_LEA_H_ */
