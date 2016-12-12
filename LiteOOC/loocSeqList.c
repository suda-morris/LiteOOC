/*
 ============================================================================
 Name        : loocSeqList.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 顺序表
 ============================================================================
 */

#include <loocSeqList.h>
#include <string.h>

static void loocSeqList_init(loocSeqList* cthis, int maxSize, int elementSize) {
	cthis->_maxSize = maxSize;
	cthis->_elementSize = elementSize;
	cthis->head = looc_malloc(elementSize * maxSize, "loocSeqList_Entry",
	looc_file_line);
}

static looc_bool loocSeqList_insert(loocSeqList* cthis, void* data) {
	if (cthis->length >= cthis->_maxSize) {
		return looc_false;
	} else {
		memcpy((cthis->head + (cthis->length) * (cthis->_elementSize)), data,
				cthis->_elementSize);
		cthis->length++;
		return looc_true;
	}
}

static looc_bool loocSeqList_remove(loocSeqList* cthis, int location) {
	if (location >= cthis->length || location < 0) {
		return looc_false;
	} else {
		memmove(location * (cthis->_elementSize) + cthis->head,
				(location + 1) * (cthis->_elementSize) + cthis->head,
				(cthis->length - location - 1) * cthis->_elementSize);
		cthis->length--;
		return looc_true;
	}
}

static void* loocSeqList_getElement(loocSeqList* cthis, int location) {
	if (location < 0 || location >= cthis->length) {
		return NULL;
	} else {
		return (cthis->head + location * cthis->_elementSize);
	}
}

static looc_bool loocSeqList_modify(loocSeqList* cthis, int location,
		void* newData) {
	if (location >= cthis->length || location < 0) {
		return looc_false;
	} else {
		memcpy(cthis->head + location * cthis->_elementSize, newData,
				cthis->_elementSize);
		return looc_true;
	}
}

static void loocSeqList_finalize(loocObject* object) {
	loocSeqList* list = SUB_PTR(object, loocObject, loocSeqList);
	if (list->head) {
		looc_free(list->head);
	}
}

/**
 * loocSeqList的构造函数
 */
CTOR(loocSeqList)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->length = 0;
	cthis->_maxSize = LOOC_SEQLIST_DEFAULT_SIZE;
	cthis->head = NULL;
	cthis->_elementSize = 1;
	/* 成员函数的绑定 */
	FUNCTION_SETTING(init, loocSeqList_init);
	FUNCTION_SETTING(insert, loocSeqList_insert);
	FUNCTION_SETTING(remove, loocSeqList_remove);
	FUNCTION_SETTING(getElement, loocSeqList_getElement);
	FUNCTION_SETTING(modify, loocSeqList_modify);
	FUNCTION_SETTING(loocObject.finalize, loocSeqList_finalize);END_CTOR

/**
 * loocSeqList的析构函数
 */
DTOR(loocSeqList)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	loocObject_dtor(SUPER_PTR(cthis, loocObject));
END_DTOR
