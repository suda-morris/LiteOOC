/*
 ============================================================================
 Name        : loocQueue.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 队列
 ============================================================================
 */

#include <loocQueue.h>
#include <string.h>

/**
 * 初始化队列
 * @param cthis       当前队列对象指针
 * @param maxSize     队列中最多的元素数量
 * @param elementSize 队列中元素的大小
 */
static void loocQueue_init(loocQueue* cthis, int maxSize, int elementSize) {
	cthis->_elementSize = elementSize;
	cthis->_maxSize = maxSize;
	cthis->queue_pool = looc_malloc(maxSize * elementSize, "loocQueue_Entry",
	looc_file_line);
}

/**
 * 入队操作（从队尾）
 * @param  cthis 当前队列对象指针
 * @param  data  入队数据指针
 * @return       成功返回true，失败返回false
 */
static looc_bool loocQueue_enqueue(loocQueue* cthis, void* data) {
	/* 判断是否队列已满 */
	if (cthis->length >= cthis->_maxSize || data == NULL) {
		return looc_false;
	} else {
		/* 数据拷贝实现入队操作 */
		memcpy(cthis->queue_pool + (cthis->rear) * (cthis->_elementSize), data,
				cthis->_elementSize);
		cthis->rear = (cthis->rear + 1) % (cthis->_maxSize);
		cthis->length++;
		return looc_true;
	}
}

/**
 * 出队操作
 * @param cthis 当前队列对象指针
 * @return      成功返回出队的数据指针，失败返回NULL
 */
static void* loocQueue_dequeue(loocQueue* cthis) {
	void* ret;
	/* 判断是否队列为空 */
	if (cthis->length <= 0) {
		return NULL;
	} else {
		ret = cthis->queue_pool + (cthis->front) * (cthis->_elementSize);
		cthis->front = (cthis->front + 1) % (cthis->_maxSize);
		cthis->length--;
		return ret;
	}
}

/**
 * 队列销毁函数
 * @param object loocObject对象指针
 */
static void loocQueue_finalize(loocObject* object) {
	loocQueue* queue = SUB_PTR(object, loocObject, loocQueue);
	if (queue->queue_pool) {
		looc_free(queue->queue_pool);
	}
	queue->length = 0;
	queue->front = 0;
	queue->rear = 0;
}

/**
 * loocQueue的构造函数
 */
CTOR(loocQueue)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_elementSize = 1;
	cthis->_maxSize = LOOC_QUEUE_DEFAULT_SIZE;
	cthis->front = 0;
	cthis->rear = 0;
	cthis->length = 0;
	/* 成员函数的绑定 */
	FUNCTION_SETTING(init, loocQueue_init);
	FUNCTION_SETTING(enqueue, loocQueue_enqueue);
	FUNCTION_SETTING(dequeue, loocQueue_dequeue);
	FUNCTION_SETTING(loocObject.finalize, loocQueue_finalize);END_CTOR

/**
 * loocQueue的析构函数
 */
DTOR(loocQueue)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
