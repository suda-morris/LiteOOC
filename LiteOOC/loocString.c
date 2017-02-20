/*
 ============================================================================
 Name        : loocString.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 字符串
 ============================================================================
 */
#include "loocString.h"
#include <string.h>
#include <stdio.h>

/**
 * 字符串初始化
 * @param cthis 当前字符串对象指针
 * @param cstr 	C语言字符数组
 */
static void loocString_init(loocString* cthis, const char* cstr) {
	cthis->string_pool = (char*) looc_malloc(sizeof(char) * strlen(cstr) * 2,
			"loocString_pool", looc_file_line);
	strncpy(cthis->string_pool, cstr, strlen(cstr));
}

/**
 * 获取当前字符串长度
 * @param cthis 当前字符串对象指针
 * @return		返回字符串长度
 */
static int loocString_length(loocString* cthis) {
	return strlen((const char*) cthis->string_pool);
}

/**
 * 获取C语言原始字符数组
 * @param  cthis 当前字符串对象指针
 * @return       返回原始字符数组
 */
static char* loocString_c_str(loocString* cthis) {
	return cthis->string_pool;
}

/**
 * 字符串打印函数
 * @param cthis 当前字符串对象指针
 */
static void loocString_print(loocString* cthis) {
	printf("%s\r\n", cthis->string_pool);
}

/**
 * 从指定位置复制指定长度给子字符串
 * @param  cthis 当前字符串
 * @param  off   指定偏移位置
 * @param  len   指定要复制的长度
 * @return       返回子字符串
 */
static loocString* loocString_substr(loocString* cthis, int off, int len) {
	if (off < 0) {
		return NULL;
	}
	if (off + len > cthis->length(cthis) || len == 0) {
		len = cthis->length(cthis) - off;
	}
	loocString* child = loocString_new(looc_file_line);
	child->string_pool = (char*) looc_malloc(sizeof(char) * len * 2,
			"loocString_substr", looc_file_line);
	memcpy(child->string_pool, cthis->string_pool + off, len);
	child->string_pool[len] = '\0';
	return child;
}

/**
 * 字符串正序查找
 * @param  cthis 当前字符串对象
 * @param  s     要查找的字符串
 * @return       查找成功返回返回子字符串的偏移，失败返回-1
 */
static int loocString_find(loocString* cthis, const char* s) {
	char* p = NULL;
	int fLen = 0;
	int cLen = 0;
	int i = 0, j = 0;
	/* 两个字符串不能空 */
	if (cthis == NULL || s == NULL || cthis->string_pool == NULL) {
		return -1;
	}
	fLen = cthis->length(cthis);
	cLen = strlen(s);
	/* 待查字符串不能过分长 */
	if (cLen > fLen) {
		return -1;
	}
	while (i <= fLen - cLen) {
		p = cthis->string_pool + i;
		/* 找到头 */
		if (*p == *s) {
			for (j = 1; j < cLen; j++) {
				if (*(p + j) != *(s + j)) {
					break;
				}
			}
			if (j >= cLen) {
				break;
			}
		}
		i++;
	}
	if (i > fLen - cLen) {
		return -1;
	} else {
		return i;
	}
}

/**
 * 字符串逆序查找
 * @param  cthis 当前字符串对象
 * @param  s     要查找的字符串
 * @return       查找成功返回返回子字符串的偏移，失败返回-1
 */
static int loocString_rfind(loocString* cthis, const char* s) {
	char* p = NULL;
	int fLen = 0;
	int cLen = 0;
	int i = 0, j = 0;
	/* 两个字符串不能空 */
	if (cthis == NULL || s == NULL || cthis->string_pool == NULL) {
		return -1;
	}
	fLen = cthis->length(cthis);
	cLen = strlen(s);
	/* 待查字符串不能过分长 */
	if (cLen > fLen) {
		return -1;
	}
	while (i <= fLen - cLen) {
		p = cthis->string_pool + fLen - cLen - i;
		/* 找到头 */
		if (*p == *s) {
			for (j = 1; j < cLen; j++) {
				if (*(p + j) != *(s + j)) {
					break;
				}
			}
			if (j >= cLen) {
				break;
			}
		}
		i++;
	}
	if (i > fLen - cLen) {
		return -1;
	} else {
		return fLen - cLen - i;
	}
}

/**
 * 字符串销毁函数
 * @param object loocObject对象指针
 */
static void loocString_finalize(loocObject* object) {
	loocString* str = SUB_PTR(object, loocObject, loocString);
	if (str->string_pool) {
		looc_free(str->string_pool);
	}
}

/**
 * loocString的构造函数
 */
CTOR(loocString)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->string_pool = NULL;
	/* 成员函数的绑定 */
	FUNCTION_SETTING(init, loocString_init);
	FUNCTION_SETTING(length, loocString_length);
	FUNCTION_SETTING(c_str, loocString_c_str);
	FUNCTION_SETTING(substr, loocString_substr);
	FUNCTION_SETTING(find, loocString_find);
	FUNCTION_SETTING(rfind, loocString_rfind);
	FUNCTION_SETTING(print, loocString_print);
	FUNCTION_SETTING(loocObject.finalize, loocString_finalize);END_CTOR

/**
 * loocString的析构函数
 */
DTOR(loocString)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR

