/*
 ============================================================================
 Name        : loocHeap.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 堆（优先队列，也是一种完全二叉树）
 ============================================================================
 */

#ifndef LITEOOC_LOOCHEAP_H_
#define LITEOOC_LOOCHEAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <looc.h>
#include "loocObject.h"

	/* 堆的默认大小 */
#define LOOC_HEAP_DEFAULT_SIZE			(20)

#define LOOC_MAX_HEAP					(0)
#define LOOC_MIN_HEAP					(1)

	/**
	 * 堆的具体类
	 */
	CLASS(loocHeap) {
		/* 继承自loocObject */
		EXTENDS(loocObject);
		/* 堆中每个元素的大小 */
		int _elementSize;
		/* 堆的容量 */
		int _maxSize;
		/* 内存池,存储堆元素的数组 */
		void* heap_pool;
		/* 堆的有效长度 */
		int length;
		/* 堆的属性：最大堆还是最小堆 */
		int option;
		/* 初始化一个堆*/
		void (*init)(loocHeap* cthis, int opt, int maxSize, int elementSize,
				int (*compareStrategy)(void* old, void* new));
		/* 插入操作 */
		looc_bool (*insert)(loocHeap* cthis, void* data);
		/* 获取根节点元素（最值） */
		void* (*getRoot)(loocHeap* cthis);
		/* 删除根节点元素，并维护堆的性质 */
		looc_bool (*deleteRoot)(loocHeap* cthis);
	};

#ifdef __cplusplus
}
#endif
#endif /* LITEOOC_LOOCHEAP_H_ */
