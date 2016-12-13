/*
 ============================================================================
 Name        : loocDoubleList.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 双向链表
 ============================================================================
 */

#ifndef LITEOOC_LOOCDOUBLELIST_H_
#define LITEOOC_LOOCDOUBLELIST_H_

#include <looc.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "loocObject.h"
	/**
	 * 双向链表数据节点具体类
	 */
	CLASS(loocDoubleListNode) {
		/* 继承自loocObject类  */
		EXTENDS(loocObject);
		/* 数据域，指向实际的数据 */
		void* _data;
		/* 双向链表中的元素大小 */
		int _elementSize;
		/* 指向下一个节点 */
		loocDoubleListNode* next;
		/* 指向前一个节点 */
		loocDoubleListNode* prior;
		/* 初始化一个双向链表节点*/
		void (*init)(loocDoubleListNode* cthis, int elementSize, void* data);
	};

#ifdef __cplusplus
}
#endif
#endif /* LITEOOC_LOOCDOUBLELIST_H_ */
