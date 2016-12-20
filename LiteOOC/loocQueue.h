/*
 ============================================================================
 Name        : loocQueue.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 队列
 ============================================================================
 */

#ifndef LITEOOC_LOOCQUEUE_H_
#define LITEOOC_LOOCQUEUE_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "looc.h"
#include "loocObject.h"

	/* 队列默认大小 */
#define LOOC_QUEUE_DEFAULT_SIZE		(10)

	/**
	 * 队列的具体类
	 */
	CLASS(loocQueue) {
		/* 继承自loocObject */
		EXTENDS(loocObject);
		/* 队列最大的大小 */
		int _maxSize;
		/* 队列中每个元素的大小 */
		int _elementSize;
		/* 队首 */
		int front;
		/* 队尾 */
		int rear;
		/* 队列有效长度 */
		int length;
		/* 内存池 */
		void* queue_pool;
		/* 初始化一个栈*/
		void (*init)(loocQueue* cthis, int maxSize, int elementSize);
		/* 入队操作 */
		looc_bool (*enqueue)(loocQueue* cthis, void* data);
		/* 出队操作 */
		void* (*dequeue)(loocQueue* cthis);
	};

#ifdef __cplusPlus
}
#endif

#endif /* LITEOOC_LOOCQUEUE_H_ */
