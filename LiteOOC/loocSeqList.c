/*
 ============================================================================
 Name        : loocSeqList.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 顺序表
 ============================================================================
 */

#include "loocSeqList.h"
#include <string.h>

/**
 * 初始化顺序表
 * @param cthis       当前顺序表对象指针
 * @param maxSize     顺序表最大的大小
 * @param elementSize 顺序表中每个元素的大小
 */
static void loocSeqList_init(loocSeqList* cthis, int maxSize, int elementSize) {
	cthis->_maxSize = maxSize;
	cthis->_elementSize = elementSize;
	cthis->head = looc_malloc(elementSize * maxSize, "loocSeqList_Entry",
	looc_file_line);
}

/**
 * 顺序表插入操作（末尾插入）
 * @param  cthis 当前顺序表对象指针
 * @param  data  往顺序表中待插入的数据
 * @return       成功返回true，失败返回false
 */
static looc_bool loocSeqList_insert(loocSeqList* cthis, void* data) {
	if (cthis->length >= cthis->_maxSize || data == NULL) {
		return looc_false;
	} else {
		/* 数据拷贝 */
		memcpy(cthis->head + (cthis->length) * (cthis->_elementSize), data,
				cthis->_elementSize);
		cthis->length++;
		return looc_true;
	}
}

/**
 * 顺序表删除元素操作
 * @param  cthis    当前顺序表对象指针
 * @param  location 待删除数据的位置,从0开始计数
 * @return          成功返回true，失败返回false
 */
static looc_bool loocSeqList_remove(loocSeqList* cthis, int location) {
	if (location >= cthis->length || location < 0) {
		return looc_false;
	} else {
		/* 把后面的数据往前移动 */
		memmove(location * (cthis->_elementSize) + cthis->head,
				(location + 1) * (cthis->_elementSize) + cthis->head,
				(cthis->length - location - 1) * cthis->_elementSize);
		cthis->length--;
		return looc_true;
	}
}

/**
 * 获取顺序表中某个位置的元素
 * @param cthis    当前顺序表对象指针
 * @param location 要获取的数据的位置
 * @return         成功返回数据的地址，失败返回NULL
 */
static void* loocSeqList_getElement(loocSeqList* cthis, int location) {
	if (location < 0 || location >= cthis->length) {
		return NULL;
	} else {
		return (cthis->head + location * (cthis->_elementSize));
	}
}

/**
 * 修改顺序表指定位置的元素
 * @param  cthis    当前顺序表对象指针
 * @param  location 待修改数据的位置
 * @param  newData  替换的新数据
 * @return          成功返回true，失败返回false
 */
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

/**
 * 顺序表实现loocObject类的finalize方法
 * @param object loocObject对象指针
 */
static void loocSeqList_finalize(loocObject* object) {
	loocSeqList* list = SUB_PTR(object, loocObject, loocSeqList);
	if (list->head) {
		looc_free(list->head);
	}
	list->length = 0;
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
	SUPER_DTOR(loocObject);END_DTOR
