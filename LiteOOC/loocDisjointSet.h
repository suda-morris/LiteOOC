/*
 ============================================================================
 Name        : loocDisjointSet.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 并查集(不相交集)
 ============================================================================
 */

#ifndef LITEOOC_LOOCDISJOINTSET_H_
#define LITEOOC_LOOCDISJOINTSET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <looc.h>
#include "loocObject.h"

#define LOOC_DISJOINT_SET_DEFAULT_SIZE	(20)
	/**
	 * 并查集的具体类
	 */
	CLASS(loocDisjointSet) {
		/* 继承自loocObject */
		EXTENDS(loocObject);
		/* 并查集中每个元素的大小 */
		int _elementSize;
		/* 并查集的容量 */
		int _maxSize;
		/* 数据域内存池 */
		void* data_pool;
		/* 双亲表示法，每个节点的父节点下标 */
		int* parent;
		/* 并查集的有效长度 */
		int length;
		/* 初始化一个堆*/
		void (*init)(loocDisjointSet* cthis, int maxSize, int elementSize);
		/* 插入操作 */
		looc_bool (*insert)(loocDisjointSet* cthis, void* data);
		/* 集合查找操作（查找某个元素所在集合，用根节点下标表示） */
		int (*find)(loocDisjointSet* cthis, void* x);
		/* 集合并操作 */
		looc_bool (*Union)(loocDisjointSet* cthis, void* x1, void* x2);
	};

#ifdef __cplusplus
}
#endif

#endif /* LITEOOC_LOOCDISJOINTSET_H_ */
