/*
 ============================================================================
 Name        : loocSingleList.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 单向链表
 ============================================================================
 */

#ifndef LITEOOC_LOOCSINGLELIST_H_
#define LITEOOC_LOOCSINGLELIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "looc.h"
#include "loocObject.h"

	/**
	 * 单向链表数据节点具体类
	 */
	CLASS(loocSingleListNode) {
		/* 继承自loocObject类  */
		EXTENDS(loocObject);
		/* 数据域，指向实际的数据 */
		void* _data;
		/* 单向链表中的元素大小 */
		int _elementSize;
		/* 指向下一个节点 */
		loocSingleListNode* next;
		/* 初始化一个单向链表节点*/
		void (*init)(loocSingleListNode* cthis, int elementSize, void* data);
	};

	/**
	 * 单向链表具体类
	 */
	CLASS(loocSingleList) {
		/* 继承自loocObject类  */
		EXTENDS(loocObject);
		/* 单向链表中的元素大小 */
		int _elementSize;
		/* 单向链表的有效长度 */
		int length;
		/* 每个单向链表都会有一个头*/
		loocSingleListNode* head;
		/* 初始化一个单向链表 */
		void (*init)(loocSingleList* cthis, int elementSize,
				loocSingleListNode* pHead);
		/* 在指定节点后面插入操作 */
		looc_bool (*insertAt)(loocSingleList* cthis, int position,
				void* newData);
		/* 删除指定节点 */
		looc_bool (*removeAt)(loocSingleList* cthis, int position);
		/* 获取指定节点 */
		void* (*getAt)(loocSingleList* cthis, int position);
	};

#ifdef __cplusPlus
}
#endif

#endif /* LITEOOC_LOOCSINGLELIST_H_ */
