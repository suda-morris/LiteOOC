/*
 ============================================================================
 Name        : loocStack.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 栈
 ============================================================================
 */

#include <loocStack.h>
#include <string.h>

/**
 * 栈初始化
 * @param cthis       当前栈对象指针
 * @param maxSize     栈最大的大小
 * @param elementSize 栈中元素大小
 */
static void loocStack_init(loocStack* cthis, int maxSize, int elementSize) {
	cthis->_elementSize = elementSize;
	cthis->_maxSize = maxSize;
	cthis->stack_pool = looc_malloc(maxSize * elementSize, "loocStack_Entry",
	looc_file_line);
}

/**
 * 入栈操作
 * @param  cthis 当前栈对象指针
 * @param  data  待入栈的数据指针
 * @return       成功返回true，失败返回false
 */
static looc_bool loocStack_push(loocStack* cthis, void* data) {
	/* 判断是否栈满 */
	if (cthis->top >= (cthis->_maxSize - 1) || data == NULL) {
		return looc_false;
	} else {
		cthis->top++;
		/* 数据拷贝实现从栈顶压入元素 */
		memcpy(cthis->stack_pool + (cthis->top) * (cthis->_elementSize), data,
				cthis->_elementSize);
		return looc_true;
	}
}

/**
 * 出栈操作
 * @param cthis 当前栈对象指针
 * @return      成功返回出栈数据指针，失败返回NULL
 */
static void* loocStack_pop(loocStack* cthis) {
	if (cthis->top == -1) {
		return NULL;
	} else {
		cthis->top--;
		return (cthis->stack_pool + (cthis->top + 1) * (cthis->_elementSize));
	}
}

/**
 * 栈的销毁函数
 * @param object loocObject对象指针
 */
static void loocStack_finalize(loocObject* object) {
	loocStack* stack = SUB_PTR(object, loocObject, loocStack);
	if (stack->stack_pool) {
		looc_free(stack->stack_pool);
	}
	stack->top = -1;
}

/**
 * loocStack的构造函数
 */
CTOR(loocStack)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_elementSize = 1;
	cthis->_maxSize = LOOC_STACK_DEFAULT_SIZE;
	cthis->top = -1;
	/* 成员函数的绑定 */
	FUNCTION_SETTING(init, loocStack_init);
	FUNCTION_SETTING(push, loocStack_push);
	FUNCTION_SETTING(pop, loocStack_pop);
	FUNCTION_SETTING(loocObject.finalize, loocStack_finalize);END_CTOR

/**
 * loocStack的析构函数
 */
DTOR(loocStack)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
