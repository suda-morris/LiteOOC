/*
 ============================================================================
 Name        : loocCircularList.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 循环链表
 ============================================================================
 */

#include <loocCircularList.h>
#include <string.h>

/**
 * 初始化循环链表节点
 * @param cthis       当前循环链表节点对象指针
 * @param elementSize 循环链表节点元素大小
 * @param data        循环链表节点数据指针
 */
static void loocCircularListNode_init(loocCircularListNode* cthis,
		int elementSize, void* data) {
	cthis->_elementSize = elementSize;
	cthis->_data = looc_malloc(elementSize, "loocCircularListNode_Entry",
	looc_file_line);
	/* 拷贝数据从栈空间拷贝到堆空间 */
	memcpy(cthis->_data, data, elementSize);
}

/**
 * 循环链表节点销毁函数
 * @param object loocObject对象指针
 */
static void loocCircularListNode_finalize(loocObject* object) {
	loocCircularListNode* node = SUB_PTR(object, loocObject,
			loocCircularListNode);
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
			loocCircularListNode_delete(node->next);
		}
	}
}

/**
 * loocCircularListNode构造函数
 */
CTOR(loocCircularListNode)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	/* 成员函数绑定 */
	FUNCTION_SETTING(init, loocCircularListNode_init);
	FUNCTION_SETTING(loocObject.finalize, loocCircularListNode_finalize);END_CTOR

/**
 * loocCircularListNode析构函数
 */
DTOR(loocCircularListNode)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR

static void loocCircularList_init(loocCircularList* cthis, int elementSize,
		loocCircularListNode* pHead) {
	cthis->_elementSize = elementSize;
	if (pHead) {
		cthis->head = pHead;
		cthis->length++;
		pHead->next = pHead;
		pHead->loocObject._use++;
	}
}

/**
 * 向循环链表指定位置插入数据
 * @param  cthis    当前循环链表对象指针
 * @param  position 将数据插入到哪个位置
 * @param  newData  待插入节点的数据指针
 * @return          成功返回true，失败返回false
 */
static looc_bool loocCircularList_insertAt(loocCircularList* cthis,
		int position, void* newData) {
	loocCircularListNode* p = cthis->head;
	loocCircularListNode* node;
	int i;
	/* 如果当前链表没有任何数据 */
	if (cthis->length == 0) {
		node = loocCircularListNode_new(looc_file_line);
		node->init(node, cthis->_elementSize, newData);
		cthis->head = node;
		node->next = node;
		node->loocObject._use++;
		cthis->length++;
		return looc_true;
	}
	if (position >= cthis->length || position < 0) {
		return looc_false;
	}
	for (i = 0; i < position; i++) {
		p = p->next;
	}
	node = loocCircularListNode_new(looc_file_line);
	node->init(node, cthis->_elementSize, newData);
	node->next = p->next;
	p->next = node;
	/* 增加引用计数 */
	node->loocObject._use++;
	cthis->length++;
	return looc_true;
}

/**
 * 删除循环链表指定位置的节点
 * @param  cthis    当前循环链表对象指针
 * @param  position 待删除节点的位置
 * @return          成功返回true，失败返回false
 */
static looc_bool loocCircularList_removeAt(loocCircularList* cthis,
		int position) {
	loocCircularListNode* p = cthis->head;
	loocCircularListNode* q = p;
	int i;
	if (position >= cthis->length || position < 0) {
		return looc_false;
	}
	if (position == 0) {
		for (i = 0; i < cthis->length - 1; i++) {
			p = p->next;
		}
		p->next = q->next;
		cthis->head = q->next;
		q->next = NULL;
		loocCircularListNode_delete(q);
	} else {
		for (i = 0; i < position - 1; i++) {
			p = p->next;
		}
		q = p->next;
		p->next = q->next;
		q->next = NULL;
		loocCircularListNode_delete(q);
	}
	cthis->length--;
	return looc_true;
}

/**
 * 获取循环链表指定位置处的元素
 * @param cthis    当前循环链表对象指针
 * @param position 指定获取元素的位置
 * @return         成功返回数据指针，失败返回NULL
 */
static void* loocCircularList_getAt(loocCircularList* cthis, int position) {
	loocCircularListNode* p = cthis->head;
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
 * 循环链表对象销毁函数
 * @param object loocObject对象指针
 */
static void loocCircularList_finalize(loocObject* object) {
	loocCircularList* list = SUB_PTR(object, loocObject, loocCircularList);
	loocCircularListNode* node = NULL;
	if (list->head) {
		node = list->head->next;
		/* 切断环形链表 */
		list->head->next = NULL;
		if (node->loocObject._use) {
			node->loocObject._use--;
		}
		if (node->loocObject._use == 0) {
			loocCircularListNode_delete(node);
		}
	}
	list->length = 0;
}

/**
 * loocCircularList的构造函数
 */
CTOR(loocCircularList)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_elementSize = 1;
	cthis->head = NULL;
	cthis->length = 0;
	/* 成员函数的绑定 */
	FUNCTION_SETTING(init, loocCircularList_init);
	FUNCTION_SETTING(loocObject.finalize, loocCircularList_finalize);
	FUNCTION_SETTING(insertAt, loocCircularList_insertAt);
	FUNCTION_SETTING(removeAt, loocCircularList_removeAt);
	FUNCTION_SETTING(getAt, loocCircularList_getAt);END_CTOR

/**
 * loocCircularList的销毁函数
 */
DTOR(loocCircularList)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
