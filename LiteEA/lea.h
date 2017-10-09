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
int prime(int number, int* result);

/**
 * 寻找指定范围内的完美数
 */
int PerfectNumber(int n, int* result);

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

/**
 * 将数组循环左移num长度
 */
void shift_left(int* array, int len, int num);

/**
 * 寻找最大的N个数
 */
looc_bool maxN(int* data, int m, int N, int* result);

/**
 * 寻找最小的N个数
 */
looc_bool minN(int* data, int m, int N, int* result);

/**
 * 查找数组中的最大值和最小值
 */
looc_bool getMaxMin(int* array, int n, int* result);

/**
 * 计算整数n二进制表示中的1的个数
 */
int countBinaryOne(int n);

/**
 * 表达式求值
 */
double computeExpress(const char* express);

/**
 * 寻找数组的主元素
 */
int findMajority(int A[], int n, int* major);

/**
 * 求n个元素的所有不同的排列
 */
void Permutation(int A[], int n, int k);

/**
 * 求n个元素的所有不同的排列中的第k个排列(从小到大排序)
 */
void Kth_Permutation(int A[], int n, int k);

/**
 * 从数组A中寻找和为s的两个数
 */
void findTwoSum(int A[], int n, int s);

/**
 * 从数组A中寻找和为s的正整数序列
 */
void findSequenceSum(int A[], int n, int s);

/**
 * 删除数组中重复的元素
 */
int removeDuplicate(int A[], int n);

/**
 * 返回needle在haystack中出现的第一个位置
 */
int strStr(const char* haystack, const char* needle);

/**
 * 给定压栈序列，判断弹栈序列是否合法
 */
int checkPopSequence(int pushSequence[], int popSequence[], int n);

/**
 * 判断字符串中的括号是否匹配
 */
int isValidParentheses(const char* str);

/**
 * 简化路径
 */
char* simplifyPath(char* path);

/**
 * 判断一个数是否是丑数
 */
int isUgly(int num);

/**
 * 最大子序列和
 */
int maxSubseqSum(int A[], int N, int* start, int* end);

/**
 * 最大矩形面积
 */
int maxRectArea(int h[], int n, int* start, int* end);

/**
 * 找出函数的最宽尖峰
 */
int findMaxPulse(int A[], int n, int* start, int* end);

/**
 * 统计阶乘末尾0的个数
 */
int countZeroInFactorial(int n);

#ifdef __cplusPlus
}
#endif

#endif /* LITEEA_LEA_H_ */
