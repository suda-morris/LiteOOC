/*
 ============================================================================
 Name        : loocCircularList.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 循环链表
 ============================================================================
 */

#ifndef LITEOOC_LOOCCIRCULARLIST_H_
#define LITEOOC_LOOCCIRCULARLIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <looc.h>
#include "loocObject.h"

	/**
	 * 循环链表节点具体类
	 */
	CLASS(loocCircularListNode) {
		/* 继承自loocObject类  */
		EXTENDS(loocObject);
		/* 数据域，指向实际的数据 */
		void* _data;
		/* 循环链表中的元素大小 */
		int _elementSize;
		/* 指向下一个节点 */
		loocCircularListNode* next;
		/* 初始化循环链表节点*/
		void (*init)(loocCircularListNode* cthis, int elementSize, void* data);
	};

	/**
	 * 循环链表具体类
	 */
	CLASS(loocCircularList) {
		/* 继承自loocObject类  */
		EXTENDS(loocObject);
		/* 循环链表中的元素大小 */
		int _elementSize;
		/* 循环链表的有效长度 */
		int length;
		/* 每个循环链表都会有一个头*/
		loocCircularListNode* head;
		/* 初始化一个循环链表 */
		void (*init)(loocCircularList* cthis, int elementSize,
				loocCircularListNode* pHead);
		/* 在指定节点后面插入操作 */
		looc_bool (*insertAt)(loocCircularList* cthis, int position,
				void* newData);
		/* 删除指定节点 */
		looc_bool (*removeAt)(loocCircularList* cthis, int position);
		/* 获取指定节点数据 */
		void* (*getAt)(loocCircularList* cthis, int position);
	};

#ifdef __cplusplus
}
#endif

#endif /* LITEOOC_LOOCCIRCULARLIST_H_ */
