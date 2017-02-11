/*
 ============================================================================
 Name        : loocDisjointSet.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 并查集(不相交集)
 ============================================================================
 */
#include "loocDisjointSet.h"
#include <string.h>

/**
 * 初始化并查集
 * @param cthis       当前并查集对象指针
 * @param maxSize     并查集最大容量
 * @param elementSize 并查集中数据的大小
 */
static void loocDisjointSet_init(loocDisjointSet* cthis, int maxSize,
		int elementSize) {
	int i;
	cthis->_elementSize = elementSize;
	cthis->_maxSize = maxSize;
	cthis->data_pool = looc_malloc(elementSize * maxSize,
			"loocDisjointSet_data_pool", looc_file_line);
	cthis->parent = (int*) looc_malloc(sizeof(int) * maxSize,
			"loocDisjointSet_parent",
			looc_file_line);
	/* 每个元素都是独立的个体 */
	for (i = 0; i < cthis->_maxSize; i++) {
		cthis->parent[i] = -1;
	}
}

/**
 * 往并查集中插入数据
 * @param  cthis 当前并查集对象指针
 * @param  data  待插入数据的指针
 * @return       成功返回true，失败返回false
 */
static looc_bool loocDisjointSet_insert(loocDisjointSet* cthis, void* data) {
	if (cthis->length >= cthis->_maxSize || data == NULL) {
		return looc_false;
	} else {
		memcpy(cthis->data_pool + cthis->length * cthis->_elementSize, data,
				cthis->_elementSize);
		cthis->length++;
		return looc_true;
	}
}

/**
 * 查找值为x的元素所属的集合
 * @param  cthis 当前并查集对象指针
 * @param  x     待查找的数据
 * @return       成功返回待查找数据所在集合的根节点下标，失败返回-1
 */
static int loocDisjointSet_find(loocDisjointSet* cthis, void* x) {
	int i, j;
	char* a = (char*) x;
	char* b;
	/* 寻找值为x元素的位置i */
	for (i = 0; i < cthis->length; i++) {
		b = (char*) (cthis->data_pool + i * cthis->_elementSize);
		for (j = 0; j < cthis->_elementSize; j++) {
			if (a[j] != b[j]) {
				break;
			}
		}
		if (j >= cthis->_elementSize) {
			break;
		}
	}
	/* 查不到该元素x */
	if (i >= cthis->_maxSize) {
		return -1;
	}
	/* 寻找根节点下标 */
	while (cthis->parent[i] >= 0) {
		i = cthis->parent[i];
	}
	return i;
}

/**
 * 并查集中的x1和x2数据的并操作，一般是x1->x2
 * @param cthis 当前并查集对象
 * @param x1    数据1
 * @param x2    数据2
 * @return      成功返回true，失败返回false
 */
static looc_bool loocDisjointSet_Union(loocDisjointSet* cthis, void* x1,
		void* x2) {
	int root1, root2;
	/* 寻找x1和x2的根节点下标 */
	root1 = loocDisjointSet_find(cthis, x1);
	root2 = loocDisjointSet_find(cthis, x2);
	if (root1 < 0 || root2 < 0) {
		return looc_false;
	}
	if (root1 != root2) {
		/* 集合1比较大(所以数值上负的更多)，所以集合2并入集合1中 */
		if (cthis->parent[root1] < cthis->parent[root2]) {
			cthis->parent[root1] += cthis->parent[root2];
			cthis->parent[root2] = root1;
		} else {
			cthis->parent[root2] += cthis->parent[root1];
			cthis->parent[root1] = root2;
		}
	}
	return looc_true;
}

/**
 * 堆对象的销毁函数
 * @param object loocObject对象指针
 */
static void loocDisjointSet_finalize(loocObject* object) {
	loocDisjointSet* set = SUB_PTR(object, loocObject, loocDisjointSet);
	if (set->data_pool) {
		looc_free(set->data_pool);
	}
	if (set->parent) {
		looc_free(set->parent);
	}
	set->length = 0;
}

/**
 * loocDisjointSet的构造函数
 */
CTOR(loocDisjointSet)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_elementSize = 1;
	cthis->_maxSize = LOOC_DISJOINT_SET_DEFAULT_SIZE;
	cthis->length = 0;
	cthis->data_pool = NULL;
	cthis->parent = NULL;
	/* 成员函数绑定 */
	FUNCTION_SETTING(init, loocDisjointSet_init);
	FUNCTION_SETTING(insert, loocDisjointSet_insert);
	FUNCTION_SETTING(find, loocDisjointSet_find);
	FUNCTION_SETTING(Union, loocDisjointSet_Union);
	FUNCTION_SETTING(loocObject.finalize, loocDisjointSet_finalize);END_CTOR

/**
 * loocDisjointSet的析构函数
 */
DTOR(loocDisjointSet)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
