/*
 ============================================================================
 Name        : loocHashMap.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 哈希表（散列表）
 ============================================================================
 */

#include <loocHashMap.h>
#include <string.h>

/**
 * 初始化一个散列表
 * @param  cthis 		当前散列表对象指针
 * @param  maxSize  	散列表最大容量
 * @param  elementSize  散列表元素大小
 * @param  hash  		用户自定义的hash函数
 */
static void loocHashMap_init(loocHashMap* cthis, int maxSize, int elementSize,
		int (*hash)(loocHashMap* cthis, void* value)) {
	cthis->_elementSize = elementSize;
	cthis->_maxSize = maxSize;
	cthis->_hash = hash;
	cthis->hashPool = looc_malloc(maxSize * elementSize, "loocHashMap_Entry",
	looc_file_line);
	memset(cthis->hashPool, 0, maxSize * elementSize);
}

/**
 * 查找函数
 * @param  cthis 当前散列表对象指针
 * @param  data  数据指针
 * @return       返回所查找元素的地址序号，如果没有该元素则返回空闲的地址序号
 */
static int loocHashMap_search(loocHashMap* cthis, void* data) {
	int rp;
	int i;
	/* 计算地址 */
	int p = cthis->_hash(cthis, data);
	if (memcmp(cthis->hashPool + p * cthis->_elementSize, data,
			cthis->_elementSize) == 0) {
		return p;
	}
	/* 线型探测法处理冲突 */
	rp = (p + 1) % cthis->_maxSize;
	while (rp != p) {
		/* 没有发生冲突 */
		if (memcmp(cthis->hashPool + rp * cthis->_elementSize, data,
				cthis->_elementSize) == 0) {
			return rp;
		}
		for (i = 0; i < cthis->_elementSize; i++) {
			if (*(char*) (cthis->hashPool + rp * cthis->_elementSize + i)) {
				break;
			}
		}
		/* 找到空白地址 */
		if (i >= cthis->_elementSize) {
			return rp;
		}
		/* 循环使用线性探测法寻找空白地址 */
		rp = (rp + 1) % cthis->_maxSize;
	}
	/* 查找失败且表满 */
	return -1;
}

/**
 * 哈希表插入操作
 * @param  cthis 当前哈希表对象指针
 * @param  value 待插入数据的指针
 * @return       成功返回true，失败返回false
 */
static looc_bool loocHashMap_insert(loocHashMap* cthis, void* value) {
	/* 查找指定元素，看看散列表中是否已经存在 */
	int pos;
	int i;
	static int haveZero = 0;
	if (cthis->length >= cthis->_maxSize || value == NULL) {
		return looc_false;
	}
	pos = cthis->search(cthis, value);
	/* 查找失败且表满 */
	if (pos < 0) {
		return looc_false;
	} else if (memcmp(cthis->hashPool + pos * cthis->_elementSize, value,
			cthis->_elementSize) == 0) {
		for (i = 0; i < cthis->_elementSize; i++) {
			if (*(char*) (value + i)) {
				break;
			}
		}
		/* 插入数据为0 */
		if (i >= cthis->_elementSize) {
			if (haveZero) {
				return looc_false;
			} else {
				haveZero = 1;
				cthis->length++;
				return looc_true;
			}
		} else {
			return looc_false;
		}
	} else {
		memcpy(cthis->hashPool + pos * cthis->_elementSize, value,
				cthis->_elementSize);
		cthis->length++;
		return looc_true;
	}
}

/**
 * 获取哈希表指定位置的元素
 * @param cthis    当前哈希表对象指针
 * @param location 欲获取数据的位置序号
 */
static void* loocHashMap_getAt(loocHashMap* cthis, int location) {
	if (location < 0 || location >= cthis->_maxSize) {
		return NULL;
	} else {
		return (cthis->hashPool + location * (cthis->_elementSize));
	}
}

/**
 * 哈希表销毁函数
 * @param object loocObject对象指针
 */
static void loocHashMap_finalize(loocObject* object) {
	loocHashMap* map = SUB_PTR(object, loocObject, loocHashMap);
	if (map->hashPool) {
		looc_free(map->hashPool);
	}
	map->length = 0;
}

/**
 * loocHashMap的构造函数
 */
CTOR(loocHashMap)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_elementSize = 1;
	cthis->_maxSize = LOOC_HASH_MAP_DEFAULT_SIZE;
	cthis->_hash = NULL;
	cthis->length = 0;
	/* 成员函数的绑定 */
	FUNCTION_SETTING(init, loocHashMap_init);
	FUNCTION_SETTING(search, loocHashMap_search);
	FUNCTION_SETTING(insert, loocHashMap_insert);
	FUNCTION_SETTING(getAt, loocHashMap_getAt);
	FUNCTION_SETTING(loocObject.finalize, loocHashMap_finalize);END_CTOR

/**
 * loocHashMap的析构函数
 */
DTOR(loocHashMap)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
