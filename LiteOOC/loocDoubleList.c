/*
 ============================================================================
 Name        : loocDoubleList.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 双向链表
 ============================================================================
 */

#include <loocDoubleList.h>
#include <string.h>

/**
 * 双向链表初始化
 * @param cthis       当前双向链表节点对象指针
 * @param elementSize 双向链表元素大小
 * @param data        双向链表节点数据指针
 */
static void loocDoubleListNode_init(loocDoubleListNode* cthis, int elementSize,
		void* data) {
	cthis->_elementSize = elementSize;
	cthis->_data = looc_malloc(elementSize, "loocDoubleListNode_Entry",
	looc_file_line);
	/* 拷贝数据从栈空间拷贝到堆空间 */
	memcpy(cthis->_data, data, elementSize);
}

/**
 * loocDoubleListNode的销毁函数
 * @param object loocObject对象指针
 */
static void loocDoubleListNode_finalize(loocObject* object) {
	loocDoubleListNode* node = SUB_PTR(object, loocObject, loocDoubleListNode);
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
			loocDoubleListNode_delete(node->next);
		}
	}
}

/**
 * loocDoubleListNode的构造函数
 */
CTOR(loocDoubleListNode)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_data = NULL;
	cthis->_elementSize = 1;
	cthis->next = NULL;
	cthis->prior = NULL;
	/* 成员函数绑定 */
	FUNCTION_SETTING(init, loocDoubleListNode_init);
	FUNCTION_SETTING(loocObject.finalize, loocDoubleListNode_finalize);END_CTOR

/**
 * loocDoubleListNode的析构函数
 */
DTOR(loocDoubleListNode)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR

/**
 * 初始化双向链表
 * @param cthis       当前双向链表对象指针
 * @param elementSize 双向链表元素大小
 * @param pHead       双向链表头的节点指针
 */
static void loocDoubleList_init(loocDoubleList* cthis, int elementSize,
		loocDoubleListNode* pHead) {
	loocDoubleListNode* p = pHead;
	cthis->_elementSize = elementSize;
	if (pHead) {
		cthis->head = pHead;
		while (p) {
			cthis->length++;
			p = p->next;
		}
	}
}

/**
 * 在指定节点后插入新节点
 * @param  cthis    当前双向链表对象指针
 * @param  position 欲插入节点的位置
 * @param  newData  带插入节点的数据指针
 * @return          成功返回true，失败返回false
 */
static looc_bool loocDoubleList_insertAt(loocDoubleList* cthis, int position,
		void* newData) {
	loocDoubleListNode* p = cthis->head;
	loocDoubleListNode* node;
	int i;
	/* 如果当前链表没有任何数据 */
	if (cthis->length == 0) {
		node = loocDoubleListNode_new(looc_file_line);
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
	node = loocDoubleListNode_new(looc_file_line);
	node->init(node, cthis->_elementSize, newData);
	node->next = p->next;
	p->next = node;
	/* 增加引用计数 */
	node->loocObject._use++;
	node->prior = p;
	if (node->next) {
		node->next->prior = node;
	}
	cthis->length++;
	return looc_true;
}

/**
 * 删除指定节点的数据
 * @param  cthis    当前双向链表的对象指针
 * @param  position 待删除节点的位置
 * @return          成功返回true，失败返回false
 */
static looc_bool loocDoubleList_removeAt(loocDoubleList* cthis, int position) {
	loocDoubleListNode* p = cthis->head;
	loocDoubleListNode* q = NULL;
	int i;
	if (position >= cthis->length || position < 0) {
		return looc_false;
	}
	if (position == 0) {
		cthis->head = p->next;
		if (p->next) {
			p->next->prior = NULL;
			p->next->loocObject._use--;
		}
		p->next = NULL;
		loocDoubleListNode_delete(p);
	} else {
		for (i = 0; i < (position - 1); i++) {
			p = p->next;
		}
		q = p->next;
		p->next = q->next;
		if (q->next) {
			q->next->prior = p;
		}
		q->prior = NULL;
		q->next = NULL;
		loocDoubleListNode_delete(q);
	}
	cthis->length--;
	return looc_true;
}

/**
 * 获取指定位置的节点数据
 * @param cthis    当前双向链表对象指针
 * @param position 欲获取数据的位置
 * @return         成功返回数据指针，失败返回NULL
 */
static void* loocDoubleList_getAt(loocDoubleList* cthis, int position) {
	loocDoubleListNode* p = cthis->head;
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
 * 双向链表销毁函数
 * @param object loocObject对象指针
 */
static void loocDoubleList_finalize(loocObject* object) {
	loocDoubleList* list = SUB_PTR(object, loocObject, loocDoubleList);
	if (list->head) {
		if (list->head->loocObject._use) {
			list->head->loocObject._use--;
		}
		if (list->head->loocObject._use == 0) {
			loocDoubleListNode_delete(list->head);
		}
	}
	list->length = 0;
}

/**
 * loocDoubleList的构造函数
 */
CTOR(loocDoubleList)
	SUPER_CTOR(loocObject);
	cthis->head = NULL;
	cthis->_elementSize = 1;
	cthis->length = 0;
	/* 成员函数的绑定 */
	FUNCTION_SETTING(init, loocDoubleList_init);
	FUNCTION_SETTING(getAt, loocDoubleList_getAt);
	FUNCTION_SETTING(insertAt, loocDoubleList_insertAt);
	FUNCTION_SETTING(removeAt, loocDoubleList_removeAt);
	FUNCTION_SETTING(loocObject.finalize, loocDoubleList_finalize);END_CTOR

/**
 * loocDoubleList的析构函数
 */
DTOR(loocDoubleList)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
