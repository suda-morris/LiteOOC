/*
 ============================================================================
 Name        : loocObject.c
 Author      : morris
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : 最顶层的类，万物皆为loocObject
 ============================================================================
 */

#include "loocObject.h"

ABS_CTOR(loocObject)
/* 初始引用计数为0 */
	cthis->_use = 0;
END_ABS_CTOR

DTOR(loocObject)
/* 调用子类自定义的finalize方法 */
	cthis->finalize(cthis);
END_DTOR
