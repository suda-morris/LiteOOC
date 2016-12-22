/*
 ============================================================================
 Name        : loocBinSearchTree.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 二叉查找树
 ============================================================================
 */

#ifndef LITEOOC_LOOCBINSEARCHTREE_H_
#define LITEOOC_LOOCBINSEARCHTREE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <looc.h>
#include "loocObject.h"
	/**
	 * 二叉查找树节点具体类
	 */
	CLASS(loocBinSearchTreeNode) {
		/* 继承自loocObject类  */
		EXTENDS(loocObject);
		/* 数据域，指向实际的数据 */
		void* _data;
		/* 二叉查找树中的元素大小 */
		int _elementSize;
		/* 指向左右节点 */
		loocBinSearchTreeNode* lChild;
		loocBinSearchTreeNode* rChild;
		loocBinSearchTreeNode* parent;
		/* 初始化一个二叉查找树节点*/
		void (*init)(loocBinSearchTreeNode* cthis, int elementSize, void* data);
	};

	/**
	 * 二叉查找树具体类
	 */
	CLASS(loocBinSearchTree) {
		/* 继承自loocObject类  */
		EXTENDS(loocObject);
		/* 二叉查找树中的元素大小 */
		int _elementSize;
		/* 二叉查找树节点个数 */
		int length;
		/* 每个二叉查找树都会有一个根节点*/
		loocBinSearchTreeNode* root;
		/* 初始化一个二叉查找树 */
		void (*init)(loocBinSearchTree* cthis, int elementSize,
				loocBinSearchTreeNode* pRoot,
				int (*compareStrategy)(loocBinSearchTreeNode* cthis,
						loocBinSearchTreeNode* node));
		/* 插入操作 */
		looc_bool (*insert)(loocBinSearchTree* cthis, void* data);
		/* 排序操作（实质上是中序遍历） */
		void (*inOrder)(loocBinSearchTree* cthis,
				void (*action)(loocBinSearchTreeNode* node, void* args),
				void* args);
		/* 搜索操作 */
		loocBinSearchTreeNode* (*search)(loocBinSearchTree* cthis, void* data);
		/* 删除节点操作 */
		looc_bool (*deleteNode)(loocBinSearchTree* cthis,
				loocBinSearchTreeNode* node);
	};

#ifdef __cplusplus
}
#endif

#endif /* LITEOOC_LOOCBINSEARCHTREE_H_ */
