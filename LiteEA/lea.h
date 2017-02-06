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

/**
 * 验证一个数组是否是一个二叉查找树的后序遍历的结果
 * 如果是则返回looc_true，否则返回looc_false
 */
looc_bool verifySquenceOfBST(int squence[], int length);

/**
 * 判断CPU是否是小端模式
 */
int isLittleEndian(void);

/**
 * 数制转换，直接转换成字符串
 */
void transNum(int num, int d, char result[]);

#ifdef __cplusPlus
}
#endif

#endif /* LITEEA_LEA_H_ */
