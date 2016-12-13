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
	 * 二叉树节点具体类
	 */
	CLASS(loocBinTreeNode) {
		/* 继承自loocObject类  */
		EXTENDS(loocObject);
		/* 数据域，指向实际的数据 */
		void* _data;
		/* 二叉树中的元素大小 */
		int _elementSize;
		/* 指向左右节点 */
		loocBinTreeNode* lChild;
		loocBinTreeNode* rChild;
		/* 初始化一个二叉树节点*/
		void (*init)(loocBinTreeNode* cthis, int elementSize, void* data);
		/* 增加左右子树 */
		void (*setLeftChild)(loocBinTreeNode* cthis, void* newData);
		void (*setRightChild)(loocBinTreeNode* cthis, void* newData);
	};

	/**
	 * 二叉树具体类
	 */
	CLASS(loocBinTree) {
		/* 继承自loocObject类  */
		EXTENDS(loocObject);
		/* 二叉树中的元素大小 */
		int _elementSize;
		/* 每个二叉树都会有一个根节点*/
		loocBinTreeNode* root;
		/* 初始化一个二叉树 */
		void (*init)(loocBinTree* cthis, int elementSize,
				loocBinTreeNode* pRoot);
		/* 前序遍历 */
		void (*preOrder)(loocBinTree* cthis,
				void (*action)(loocBinTreeNode* node, void* args), void* args);
		/* 中序遍历 */
		void (*inOrder)(loocBinTree* cthis,
				void (*action)(loocBinTreeNode* node, void* args), void* args);
		/* 后序遍历 */
		void (*postOrder)(loocBinTree* cthis,
				void (*action)(loocBinTreeNode* node, void* args), void* args);
	};

#ifdef __cplusplus
}
#endif

#endif /* LITEOOC_LOOCBINTREE_H_ */
