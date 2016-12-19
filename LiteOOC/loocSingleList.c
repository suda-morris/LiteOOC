/*
 ============================================================================
 Name        : loocSingleList.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 单向链表
 ============================================================================
 */

#include <loocSingleList.h>
#include <stddef.h>
#include <string.h>

/**
 * 初始化单向链表节点
 * @param cthis       当前单向链表对象指针
 * @param elementSize 单向链表节点的数据大小
 * @param data        单向链表节点的数据指针
 */
static void loocSingleListNode_init(loocSingleListNode* cthis, int elementSize,
		void* data) {
	cthis->_elementSize = elementSize;
	cthis->_data = looc_malloc(elementSize, "loocSingleListNode_Entry",
	looc_file_line);
	/* 拷贝数据从栈空间拷贝到堆空间 */
	memcpy(cthis->_data, data, elementSize);
}

/**
 * 单向链表节点的销毁
 * @param object loocObject对象指针
 */
static void loocSingleListNode_finalize(loocObject* object) {
	loocSingleListNode* node = SUB_PTR(object, loocObject, loocSingleListNode);
	/* 释放数据域的空间 */
	if (node->_data) {
		looc_free(node->_data);
	}
	/* 根据引用计数来判断是否可以删除下一个结点 */
	if (node->next) {
		if (node->next->loocObject._use) {
			node->next->loocObject._use--;
		}
		if (node->next->loocObject._use == 0) {
			loocSingleListNode_delete(node->next);
		}
	}
}

/**
 * loocSingleListNode的构造函数
 */
CTOR(loocSingleListNode)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_data = NULL;
	cthis->_elementSize = 1;
	cthis->next = NULL;
	/* 成员函数绑定 */
	FUNCTION_SETTING(init, loocSingleListNode_init);
	FUNCTION_SETTING(loocObject.finalize, loocSingleListNode_finalize);END_CTOR

/**
 * loocSingleListNode的析构函数
 */
DTOR(loocSingleListNode)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR

/**
 * 初始化单向链表
 * @param cthis       当前单向链表对象指针
 * @param elementSize 单向链表元素大小
 * @param headData    单向链表头的节点指针
 */
static void loocSingleList_init(loocSingleList* cthis, int elementSize,
		loocSingleListNode* pHead) {
	cthis->_elementSize = elementSize;
	if (pHead) {
		cthis->head = pHead;
		cthis->length++;
	}
}

/**
 * 在指定节点后面插入数据
 * @param  cthis    当前单向链表对象指针
 * @param  position 待插入数据的位置
 * @param  newData  新数据指针
 * @return          成功返回true，失败返回false
 */
static looc_bool loocSingleList_insertAt(loocSingleList* cthis, int position,
		void* newData) {
	loocSingleListNode* p = cthis->head;
	loocSingleListNode* node;
	int i;
	/* 如果当前链表没有任何数据 */
	if (cthis->length == 0) {
		node = loocSingleListNode_new(looc_file_line);
		node->init(node, cthis->_elementSize, newData);
		cthis->head = node;
		cthis->length++;
		return looc_true;
	}
	if (position >= cthis->length || position < 0) {
		return looc_false;
	}
	for (i = 0; i < position; i++) {
		p = p->next;
	}
	node = loocSingleListNode_new(looc_file_line);
	node->init(node, cthis->_elementSize, newData);
	node->next = p->next;
	p->next = node;
	/* 增加引用计数 */
	node->loocObject._use++;
	cthis->length++;
	return looc_true;
}

/**
 * 删除指定节点
 * @param  cthis    当前单向链表对象指针
 * @param  position 待删除的节点位置
 * @return          成功返回true，失败返回false
 */
static looc_bool loocSingleList_removeAt(loocSingleList* cthis, int position) {
	loocSingleListNode* p = cthis->head;
	loocSingleListNode* q = NULL;
	int i;
	if (position >= cthis->length || position < 0) {
		return looc_false;
	}
	if (position == 0) {
		cthis->head = p->next;
		if (p->next) {
			p->next->loocObject._use--;
		}
		p->next = NULL;
		loocSingleListNode_delete(p);
	} else {
		for (i = 0; i < (position - 1); i++) {
			p = p->next;
		}
		q = p->next;
		p->next = q->next;
		q->next = NULL;
		loocSingleListNode_delete(q);
	}
	cthis->length--;
	return looc_true;
}

/**
 * 获取指定位置的数据
 * @param cthis    当前单向链表对象指针
 * @param position 欲获取数据的位置
 * @return         成功数据指针，失败返回NULL
 */
static void* loocSingleList_getAt(loocSingleList* cthis, int position) {
	loocSingleListNode* p = cthis->head;
	int i;
	if (position >= cthis->length || position < 0) {
		return NULL;
	}
	for (i = 0; i < position; i++) {
		p = p->next;
	}
	return p->_data;
}

/**
 * loocSingleList对象的销毁函数
 * @param object loocObject对象指针
 */
static void loocSingleList_finalize(loocObject* object) {
	loocSingleList* list = SUB_PTR(object, loocObject, loocSingleList);
	if (list->head) {
		if (list->head->loocObject._use) {
			list->head->loocObject._use--;
		}
		if (list->head->loocObject._use == 0) {
			loocSingleListNode_delete(list->head);
		}
	}
	list->length = 0;
}

/**
 * loocSingleList的构造函数
 */
CTOR(loocSingleList)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->head = NULL;
	cthis->_elementSize = 1;
	cthis->length = 0;
	/* 成员函数的绑定 */
	FUNCTION_SETTING(init, loocSingleList_init);
	FUNCTION_SETTING(insertAt, loocSingleList_insertAt);
	FUNCTION_SETTING(removeAt, loocSingleList_removeAt);
	FUNCTION_SETTING(getAt, loocSingleList_getAt);
	FUNCTION_SETTING(loocObject.finalize, loocSingleList_finalize);END_CTOR

/**
 * loocSingleList的析构函数
 */
DTOR(loocSingleList)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR

