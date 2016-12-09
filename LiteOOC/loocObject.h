/*
 ============================================================================
 Name        : loocObject.h
 Author      : morris
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : 最顶层的类，万物皆为loocObject
 ============================================================================
 */

#ifndef LITEOOC_LOOCOBJECT_H_
#define LITEOOC_LOOCOBJECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "looc.h"

	ABS_CLASS(loocObject) {
		/* 引用计数 */
		int _use;
		/* 子类通过覆写finalize方法，实现对资源清理行为的定制 */
		void (*finalize)(loocObject* object);
	};

#ifdef __cplusplus
}
#endif

#endif /* LITEOOC_LOOCOBJECT_H_ */
