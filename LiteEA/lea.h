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

/**
 * 验证一个数组是否是一个二叉查找树的后序遍历的结果
 * 如果是则返回looc_true，否则返回looc_false
 */
looc_bool verifySquenceOfBST(int squence[], int length);

/**
 * 数制转换，直接转换成字符串
 */
void transNum(int num, int d, char result[]);

/**
 * 插入排序
 */
void insertSort(int D[], int n);

/**
 * 冒泡排序
 */
void bubbleSort(int D[], int n);

/**
 * 快速排序
 */
void quickSort(int D[], int p, int r);

#ifdef __cplusPlus
}
#endif

#endif /* LITEEA_LEA_H_ */
