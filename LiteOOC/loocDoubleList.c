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
	/* 根据引用计数来判断是否可以删除前一个结点 */
	if (node->prior) {
		if (node->prior->loocObject._use) {
			node->prior->loocObject._use--;
		}
		if (node->prior->loocObject._use == 0) {
			loocDoubleListNode_delete(node->prior);
		}
	}
}

/**
 * loocDoubleListNode构造函数
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
 * loocDoubleListNode析构函数
 */
DTOR(loocDoubleListNode)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
