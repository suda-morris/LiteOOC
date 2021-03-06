/*
 ============================================================================
 Name        : loocHeap.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 堆（优先队列，也是一种完全二叉树）
 ============================================================================
 */

#include <loocHeap.h>
#include <string.h>

/**
 * 如果new比old小，则返回-1，相等则返回0，大于则返回1
 */
static int (*compare)(void* old, void* new) = NULL;

/**
 * 初始化堆
 * @param cthis       当前堆对象指针
 * @param opt      	      堆的属性：最大堆还是最小堆
 * @param maxSize     堆中最多的元素数量
 * @param elementSize 堆中元素的大小
 * @param compare     堆中元素的比较策略,由用户去定义
 */
static void loocHeap_init(loocHeap* cthis, int opt, int maxSize,
		int elementSize, int (*compareStrategy)(void* old, void* new)) {
	cthis->option = opt;
	cthis->_elementSize = elementSize;
	cthis->_maxSize = maxSize;
	cthis->heap_pool = looc_malloc(maxSize * elementSize, "loocHeap_Entry",
	looc_file_line);
	compare = compareStrategy;
}

/**
 * 堆的插入数据操作
 * @param  cthis 当前堆对象指针
 * @param  data  待插入堆中的数据指针
 * @return       成功返回true，失败返回false
 */
static looc_bool loocHeap_insert(loocHeap* cthis, void* data) {
	/* i指向插入后堆中的最后一个元素的位置 */
	int i = cthis->length;
	/* 判断堆是否已满 */
	if (cthis->length >= cthis->_maxSize || data == NULL) {
		return looc_false;
	} else {
		while (i) {
			/* 根据最大最小堆的性质，从下往上递 */
			if (((cthis->option == LOOC_MAX_HEAP)
					&& (compare(
							cthis->heap_pool
									+ (i - 1) / 2 * cthis->_elementSize, data)
							> 0))
					|| ((cthis->option == LOOC_MIN_HEAP)
							&& (compare(
									cthis->heap_pool
											+ (i - 1) / 2 * cthis->_elementSize,
									data) < 0))) {
				memcpy(cthis->heap_pool + i * cthis->_elementSize,
						cthis->heap_pool + (i - 1) / 2 * cthis->_elementSize,
						cthis->_elementSize);
				i = (i - 1) / 2;
			} else {
				break;
			}
		}
		memcpy(cthis->heap_pool + i * cthis->_elementSize, data,
				cthis->_elementSize);
		cthis->length++;
		return looc_true;
	}
}

/**
 * 获取堆中根节点的元素
 * @param cthis 当前堆对象指针
 * @return      成功返回数据指针
 */
static void* loocHeap_getRoot(loocHeap* cthis) {
	/* 判断堆是否为空 */
	if (cthis->length == 0) {
		return NULL;
	} else {
		return cthis->heap_pool;
	}
}

/**
 * 删除堆中的根节点
 * @param  cthis 当前堆对象指针
 * @return       成功返回true，失败返回false
 */
static looc_bool loocHeap_deleteRoot(loocHeap* cthis) {
	int child, parent = 0;
	void* temp;
	/* 判断堆是否为空 */
	if (cthis->length == 0) {
		return looc_false;
	} else {
		temp = looc_malloc(cthis->_elementSize, "loocHeap_Element",
		looc_file_line);
		memcpy(temp,
				cthis->heap_pool + (cthis->length - 1) * cthis->_elementSize,
				cthis->_elementSize);
		/* 从上到下依次递归 */
		while ((parent * 2 + 1) < cthis->length) {
			/* 左子节点 */
			child = parent * 2 + 1;
			/* 如果还有右子节点，从左右节点中选择最佳的 */
			if ((child + 1) < cthis->length) {
				if (((cthis->option == LOOC_MAX_HEAP)
						&& (compare(
								cthis->heap_pool + child * cthis->_elementSize,
								cthis->heap_pool
										+ (child + 1) * cthis->_elementSize) > 0))
						|| ((cthis->option == LOOC_MIN_HEAP)
								&& (compare(
										cthis->heap_pool
												+ child * cthis->_elementSize,
										cthis->heap_pool
												+ (child + 1)
														* cthis->_elementSize)
										< 0))) {
					child++;
				}
			}
			/* 如果最后一个节点temp满足下面的条件 */
			if (((cthis->option == LOOC_MAX_HEAP)
					&& (!(compare(
							cthis->heap_pool + child * cthis->_elementSize,
							temp) < 0)))
					|| ((cthis->option == LOOC_MIN_HEAP)
							&& (!(compare(
									cthis->heap_pool
											+ child * cthis->_elementSize, temp)
									> 0)))) {
				break;
			} else {
				memcpy(cthis->heap_pool + parent * cthis->_elementSize,
						cthis->heap_pool + child * cthis->_elementSize,
						cthis->_elementSize);
				parent = child;
			}
		}
		memcpy(cthis->heap_pool + parent * cthis->_elementSize, temp,
				cthis->_elementSize);
		cthis->length--;
		looc_free(temp);
		return looc_true;
	}
}

/**
 * 堆对象的销毁函数
 * @param object loocObject对象指针
 */
static void loocHeap_finalize(loocObject* object) {
	loocHeap* heap = SUB_PTR(object, loocObject, loocHeap);
	if (heap->heap_pool) {
		looc_free(heap->heap_pool);
	}
	heap->length = 0;
}

/**
 * loocHeap的构造函数
 */
CTOR(loocHeap)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->option = LOOC_MAX_HEAP;	//默认是最大堆
	cthis->_elementSize = 1;
	cthis->_maxSize = LOOC_HEAP_DEFAULT_SIZE;
	cthis->heap_pool = NULL;
	cthis->length = 0;
	/* 成员函数绑定 */
	FUNCTION_SETTING(init, loocHeap_init);
	FUNCTION_SETTING(insert, loocHeap_insert);
	FUNCTION_SETTING(getRoot, loocHeap_getRoot);
	FUNCTION_SETTING(deleteRoot, loocHeap_deleteRoot);
	FUNCTION_SETTING(loocObject.finalize, loocHeap_finalize);END_CTOR

/**
 * loocHeap的析构函数
 */
DTOR(loocHeap)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR

