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

static void loocSingleListNode_init(loocSingleListNode* cthis, int elementSize,
		void* data) {
	cthis->_elementSize = elementSize;
	cthis->_data = looc_malloc(elementSize, "loocSingleListNode_Entry",
	looc_file_line);
	memcpy(cthis->_data, data, elementSize);
}

static void loocSingleListNode_finalize(loocObject* object) {
	loocSingleListNode* node = SUB_PTR(object, loocObject, loocSingleListNode);
	if (node->_data) {
		looc_free(node->_data);
	}
	if (node->next) {
		if (node->next->loocObject._use) {
			node->next->loocObject._use--;
		}
		if (node->next->loocObject._use == 0) {
			loocSingleListNode_delete(node->next);
		}
	}
}

CTOR(loocSingleListNode)
	SUPER_CTOR(loocObject);
	cthis->_data = NULL;
	cthis->_elementSize = 1;
	cthis->next = NULL;
	FUNCTION_SETTING(init, loocSingleListNode_init);
	FUNCTION_SETTING(loocObject.finalize, loocSingleListNode_finalize);END_CTOR

DTOR(loocSingleListNode)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR

static void loocSingleList_init(loocSingleList* cthis, int elementSize,
		loocSingleListNode* pHead) {
	cthis->_elementSize = elementSize;
	if (pHead) {
		cthis->head = pHead;
		cthis->length++;
		pHead->loocObject._use++;
	}
}

static looc_bool loocSingleList_insertAt(loocSingleList* cthis, int position,
		void* newData) {
	loocSingleListNode* p = cthis->head;
	int i;
	if (position >= cthis->length || position < 0) {
		//ToDo length==0
		return looc_false;
	}
	for (i = 0; i < position; i++) {
		p = p->next;
	}
	loocSingleListNode* node = loocSingleListNode_new(looc_file_line);
	node->init(node, cthis->_elementSize, newData);
	node->next = p->next;
	p->next = node;
	node->loocObject._use++;
	cthis->length++;
	return looc_true;
}

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
			p->next->loocObject._use++;
		}
		loocSingleListNode_delete(p);
	} else {
		for (i = 0; i < (position - 1); i++) {
			p = p->next;
		}
		q = p->next;
		p->next = q->next;
		if (q->next) {
			q->next->loocObject._use++;
		}
		loocSingleListNode_delete(q);
	}
	cthis->length--;
	return looc_true;
}

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

CTOR(loocSingleList)
	SUPER_CTOR(loocObject);
	cthis->head = NULL;
	cthis->_elementSize = 1;
	cthis->length = 0;
	FUNCTION_SETTING(init, loocSingleList_init);
	FUNCTION_SETTING(insertAt, loocSingleList_insertAt);
	FUNCTION_SETTING(removeAt, loocSingleList_removeAt);
	FUNCTION_SETTING(getAt, loocSingleList_getAt);
	FUNCTION_SETTING(loocObject.finalize, loocSingleList_finalize);END_CTOR

DTOR(loocSingleList)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR

