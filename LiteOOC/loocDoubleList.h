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

#ifdef __cplusplus
extern "C" {
#endif

#include <looc.h>
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

	/**
	 * 双向链表具体类
	 */
	CLASS(loocDoubleList) {
		/* 继承自loocObject类  */
		EXTENDS(loocObject);
		/* 每个双向链表都会有一个头*/
		loocDoubleListNode* head;
		/* 双向链表的有效长度 */
		int length;
		/* 双向链表中的元素大小 */
		int _elementSize;
		/* 初始化一个双向链表 */
		void (*init)(loocDoubleList* cthis, int elementSize,
				loocDoubleListNode* pHead);
		/* 获取指定节点 */
		void* (*getAt)(loocDoubleList* cthis, int position);
		/* 在指定节点后面插入操作 */
		looc_bool (*insertAt)(loocDoubleList* cthis, int position,
				void* newData);
		/* 删除指定节点 */
		looc_bool (*removeAt)(loocDoubleList* cthis, int position);
	};

#ifdef __cplusplus
}
#endif
#endif /* LITEOOC_LOOCDOUBLELIST_H_ */
