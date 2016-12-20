/*
 ============================================================================
 Name        : loocHashMap.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 哈希表（散列表）
 ============================================================================
 */

#ifndef LITEOOC_LOOCHASHMAP_H_
#define LITEOOC_LOOCHASHMAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "looc.h"
#include "loocObject.h"

	/* 散列表默认大小 */
#define LOOC_HASH_MAP_DEFAULT_SIZE		(10)
	/**
	 * 散列表的具体类
	 */
	CLASS(loocHashMap) {
		/* 继承自loocObject */
		EXTENDS(loocObject);
		/* 散列表最大容量 */
		int _maxSize;
		/* 散列表中每个元素的大小 */
		int _elementSize;
		/* 散列表有效长度 */
		int length;
		/* 内存池 */
		void* hashPool;
		/* 散列函数，由用户自定义 */
		int (*_hash)(loocHashMap* cthis, void* value);
		/* 初始化一个Hash表*/
		void (*init)(loocHashMap* cthis, int maxSize, int elementSize,
				int (*hash)(loocHashMap* cthis, void* value));
		/* 查找函数 */
		int (*search)(loocHashMap* cthis, void* data);
		/* 插入操作 */
		looc_bool (*insert)(loocHashMap* cthis, void* value);
		/* 获取指定位置的元素,location从0开始计数 */
		void* (*getAt)(loocHashMap* cthis, int location);
	};

#ifdef __cplusPlus
}
#endif
#endif /* LITEOOC_LOOCHASHMAP_H_ */
