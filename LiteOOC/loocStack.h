/*
 ============================================================================
 Name        : loocStack.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 栈
 ============================================================================
 */

#ifndef LITEOOC_LOOCSTACK_H_
#define LITEOOC_LOOCSTACK_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "looc.h"
#include "loocObject.h"

	/* 栈默认大小 */
#define LOOC_STACK_DEFAULT_SIZE		(10)

	/**
	 * 栈的具体类
	 */
	CLASS(loocStack) {
		/* 继承自loocObject */
		EXTENDS(loocObject);
		/* 栈最大的大小 */
		int _maxSize;
		/* 栈中每个元素的大小 */
		int _elementSize;
		/* 栈顶指针 */
		int top;
		/* 内存池 */
		void* stack_pool;
		/* 初始化一个栈*/
		void (*init)(loocStack* cthis, int maxSize, int elementSize);
		/* 入栈操作 */
		looc_bool (*push)(loocStack* cthis, void* data);
		/* 出栈操作 */
		void* (*pop)(loocStack* cthis);
	};

#ifdef __cplusPlus
}
#endif

#endif /* LITEOOC_LOOCSTACK_H_ */
