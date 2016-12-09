/*
 ============================================================================
 Name        : looc.c
 Author      : morris
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : 定义了对象记录链表的操作
 ============================================================================
 */

#include <looc.h>
#include <stdio.h>
#include <string.h>

/* 对象记录链表的每个成员 */
typedef struct LOOC_MemAllocUnit_s {
	char file[LOOC_MAX_PATH];	//文件名
	int line;					//行号
	void* addr;					//内存地址
	size_t size;				//内存块大小
	struct LOOC_MemAllocUnit_s* next;	//下一个内存块记录
} LOOC_MemAllocUnit;

static LOOC_MemAllocUnit* looc_memAllocList = NULL;

/**
 * [分配内存，保存记录]
 * @param size [对象的大小]
 * @param type [对象的类型，字符串表示]
 * @param file [创建该对象的源文件]
 * @param line [创建该对象所在源代码的行数]
 */
void* looc_malloc(size_t size, const char* type, const char* file, int line) {
	void* addr = malloc(size);
	looc_assert(addr != NULL);	//检测是否成功分配到了内存
	if (addr) {
		LOOC_MemAllocUnit* pMemAllocUnit = (LOOC_MemAllocUnit*) malloc(
				sizeof(LOOC_MemAllocUnit));
		if (!pMemAllocUnit) {
			printf("LiteOOC:error!malloc LOOC_MemAllocUnit failed.\r\n");
			exit(1);
		}

		if (strlen(file) >= LOOC_MAX_PATH) {
			printf("LiteOOC:error!file name is more than %d characters:%s.\r\n",
			LOOC_MAX_PATH, file);
			exit(1);
		}

		strncpy(pMemAllocUnit->file, file, strlen(file));
		pMemAllocUnit->line = line;
		pMemAllocUnit->addr = addr;
		pMemAllocUnit->size = size;
		pMemAllocUnit->next = looc_memAllocList;	//插入链表头
		looc_memAllocList = pMemAllocUnit;

		looc_dbginfo(
				"LiteOOC: mlloc memory in %p, size: %lu, object type: %s, file: %s, line: %d.\r\n",
				addr, size, type, file, line);
	}
	return addr;
}

/**
 * [释放对象本身的内存空间]
 * @param memblock [对象的内存地址]
 */
void looc_free(void* memblock) {
	LOOC_MemAllocUnit* preUnit = NULL;
	LOOC_MemAllocUnit* curUnit = looc_memAllocList;	//指向链表头
	while (curUnit) {
		if (curUnit->addr == memblock) {
			looc_dbginfo("LiteOOC:free memory at %p, size: %lu.\r\n",
					curUnit->addr, curUnit->size);
			if (preUnit == NULL) {	//删除链表头
				looc_memAllocList = curUnit->next;
			} else {
				preUnit->next = curUnit->next;
			}
			free(curUnit);	//删除记录
			curUnit = NULL;
			free(memblock);	//删除对象本身
			memblock = NULL;
			return;
		} else {
			preUnit = curUnit;
			curUnit = curUnit->next;
		}
	}
	if (!curUnit) {
		printf("LiteOOC:error! you attemp to free invalid memory.\r\n");
		exit(1);
	}
}

/**
 * [内存泄露报告]
 */
void looc_report(void) {
	LOOC_MemAllocUnit* curUnit = looc_memAllocList;	//指向链表头
	if (curUnit != 0) {
		printf("LiteOOC:memory leak:\r\n");
		while (curUnit) {
			printf("memory leak at: %p, size: %lu, file: %s, line: %d.\r\n",
					curUnit->addr, curUnit->size, curUnit->file, curUnit->line);
			curUnit = curUnit->next;
		}
	} else {
		printf("LiteOOC:no memory leak.\r\n");
	}
}

