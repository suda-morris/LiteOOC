/*
 ============================================================================
 Name        : loocString.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 字符串
 ============================================================================
 */

#ifndef LITEOOC_LOOCSTRING_H_
#define LITEOOC_LOOCSTRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "loocObject.h"

	/**
	 * 字符串具体类
	 */
	CLASS(loocString) {
		/* 继承自loocObject */
		EXTENDS(loocObject);
		/* 内存池 */
		char* string_pool;
		/* 初始化一个字符串*/
		void (*init)(loocString* cthis, const char* cstr);
		/* 获取字符串长度 */
		int (*length)(loocString* cthis);
		/* 返回C语言原始字符串数组 */
		char* (*c_str)(loocString* cthis);
		/* 打印一个字符串 */
		void (*print)(loocString* cthis);
		/* 从指定位置开始复制指定长度的子串 */
		loocString* (*substr)(loocString* cthis, int off, int len);
		/* 字符串正序查找 */
		int (*find)(loocString* cthis, const char* s);
		/* 字符串逆序查找 */
		int (*rfind)(loocString* cthis, const char* s);
	};

#ifdef __cplusPlus
}
#endif

#endif /* LITEOOC_LOOCSTRING_H_ */
