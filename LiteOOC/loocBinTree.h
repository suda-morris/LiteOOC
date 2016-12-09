/*
 ============================================================================
 Name        : loocBinTree.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 二叉树
 ============================================================================
 */

#ifndef LITEOOC_LOOCBINTREE_H_
#define LITEOOC_LOOCBINTREE_H_

#include <loocObject.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * 二叉树的抽象类
	 */
	ABS_CLASS(loocBinTree) {
		/* 继承自loocObject类  */
		EXTENDS(loocObject);
		/* 每个节点都有左右子树 */
		loocBinTree* left;
		loocBinTree* right;
		/* 增加左右子树,公共方法，不需要子类去实现 */
		void (*addSubLeft)(loocBinTree* cthis, loocBinTree* sleft);
		void (*addSubRight)(loocBinTree* cthis, loocBinTree* sright);
		/* 树的打印接口，需要子类自己实现,遍历方法：前序遍历（根->左->右） */
		void (*print)(loocBinTree* cthis);
	};

	/**
	 * 整型树的具体类
	 */
	CLASS(loocBinTree_Int) {
		/* 继承自loocBinTree类 */
		EXTENDS(loocBinTree);
		/* 数据 */
		int nodeValue;
		/* init方法一般会在new方法之后被调用，用来初始化成员变量 */
		void (*init)(loocBinTree_Int* cthis, int value);
	};

#ifdef __cplusplus
}
#endif

#endif /* LITEOOC_LOOCBINTREE_H_ */
