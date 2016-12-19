/*
 ============================================================================
 Name        : looc.h
 Author      : morris
 Version     : 1.0
 Copyright   : Your copyright notice
 Description : 定义了在C语言中使用面向对象的一套机制
 LiteOOC的最佳实践
 1）  继承层次不宜过深，建议最多三层
 继承层次过深，在Java/C#/C++中均不推崇，在C语言中实践面向对象的时候，尤其要遵循这一点，这样代码才能简单清爽。
 2）  尽量避免具体类继承具体类
 具体类继承具体类，不符合抽象的原则，要尽量避免。
 3）  各继承层次分别维护好自己的数据
 子类尽量不要直接访问祖先类的数据，如果确实需要访问，应当通过祖先类提供的函数，以函数调用的方式间接访问。
 4） 抽象类的定义中要去声明finalize方法，子类继承的时候要去根据自身情况去实现finalize方法
 5） 使用引用计数_use来更好的管理内存。何为引用？当对象B的地址成为了对象A的一部分，则称A引用了B，B的引用计数需要加1
 6） 具有多层继承关系的类，他们只有一个公共的finalize方法，并且由最终的子类去实现，绑定给loocObject
 ============================================================================
 */

#ifndef LITEOOC_INCLUDE_LOOC_H_
#define LITEOOC_INCLUDE_LOOC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

/* 程序源文件所在路径的长度最大值 */
#define LOOC_MAX_PATH			(128)

/* LiteOOC中使用的断言 */
#define looc_assert				assert

/* 计算子成员的偏移，实现多态的关键 */
#define looc_offsetof			offsetof

/* 打印调试功能 */
#define looc_dbginfo			printf

/* LiteOOC中分配对象内存的时候使用 ，会将开辟的内存记录在对象链表中 */
extern void* looc_malloc(size_t size, const char* type, const char* file,
		int line);

/* 释放某个对象的内存空间，并从对象记录链表中删除 */
extern void looc_free(void* memblock);

/* 报告内存泄露情况,，通常在程序运行结束前调用 */
extern void looc_report();

/* 用于记录创建新对象所在源代码的位置 */
#define looc_file_line			__FILE__, __LINE__
#define looc_file_line_param	const char* file, int line

/* 自定义布尔类型 */
typedef enum looc_bool {
	looc_false = 0, looc_true = 1
} looc_bool;

/* 抽象类 */
#define ABS_CLASS(type)						\
	typedef struct type##_s type;			\
	extern void type##_ctor(type* cthis); 	\
	extern void type##_dtor(type* cthis);  	\
	extern void type##_delete(type* cthis);	\
	struct type##_s

/* 具体类 */
#define CLASS(type)									\
	typedef struct type##_s type;					\
	extern type* type##_new(looc_file_line_param);	\
	extern void type##_ctor(type* cthis);			\
	extern void type##_dtor(type* cthis);			\
	extern void type##_delete(type* cthis);			\
	struct type##_s

/**
 * 具体类的构造函数（实现具体类的new方法）
 * 1. 开辟对象内存，并加入对象链表中
 * 2. 在new方法中调用ctor方法，ctor方法(主要是完成一些成员函数的绑定工作)由用户自己实现
 */
#define CTOR(type)									\
	type* type##_new(const char* file, int line){ 	\
		type* cthis;								\
		cthis = (type*)looc_malloc(sizeof(type),#type,file,line);	\
		if(!cthis){									\
			return NULL;							\
		}											\
		type##_ctor(cthis);							\
		return cthis;								\
	}												\
	void type##_ctor(type* cthis){
#define END_CTOR		}

/**
 * 抽象类的构造函数（抽象类没有new方法，只有ctor方法）
 * 1. ctor方法由用户自己实现(主要是完成一些成员函数的绑定工作)
 * 2. 子类（具体类）的构造函数中要显示地调用抽象父类的ctor方法
 */
#define ABS_CTOR(type)					\
	void type##_ctor(type* cthis){
#define END_ABS_CTOR 	}

/**
 * 析构函数（实现delete方法：）
 * 1. 在delete方法中调用dtor方法，dtor方法(主要是释放子成员的内存空间)由用户自己实现
 * 2. 将对象从当前对象链表中删除,并且将对象本身的内存空间释放
 */
#define DTOR(type)						\
	void type##_delete(type* cthis){	\
		type##_dtor(cthis);				\
		looc_free((void*)cthis);		\
	}									\
	void type##_dtor(type* cthis){
#define END_DTOR 		}

/* 完成成员函数的绑定工作，必须在CTOR和END_CTOR之间调用 */
#define FUNCTION_SETTING(f1,f2)			\
	cthis->f1 = f2

/* 继承某个类,可以是具体类，也可以是抽象类，必须在CLASS内部调用 */
#define EXTENDS(type)		struct type##_s type

/* 向上转型,cthis为子类对象指针,father为父类名,返回父类对象指针 */
#define SUPER_PTR(cthis, father)		\
	((father*)(&(cthis->father)))
#define SUPER_PTR_2(cthis, father, grandfather)		\
	SUPER_PTR(SUPER_PTR(cthis, father), grandfather)
#define SUPER_PTR_3(cthis, father, grandfather, greatgrandfather)	\
	SUPER_PTR(SUPER_PTR_2(cthis, father, grandfather), greatgrandfather)

/* 调用父类的ctor方法,father为父类名,必须是ABS_CTOR或者CTOR下面紧跟的第一条语句 */
#define SUPER_CTOR(father)				\
	father##_ctor(SUPER_PTR(cthis,father))

/* 调用父类的dtor方法,father为父类名,必须是ABS_CTOR或者CTOR下面紧跟的第一条语句 */
#define SUPER_DTOR(father)				\
	father##_dtor(SUPER_PTR(cthis,father))

/* 向下转型，selfptr为父类对象指针，self为父类名，child为子类名 */
#define SUB_PTR(selfptr,self,child)		\
	((child*)((char*)selfptr-looc_offsetof(child,self)))
#define SUB_PTR_2(selfptr, self, child, grandchild)\
	SUB_PTR(SUB_PTR(selfptr, self, child), child, grandchild)
#define SUB_PTR_3(selfptr, self, child, grandchild, greatgrandchild)\
	SUB_PTR(SUB_PTR_2(selfptr, self, child, grandchild), grandchild, greatgrandchild)

#ifdef __cplusplus
}
#endif

#endif /* LITEOOC_INCLUDE_LOOC_H_ */
