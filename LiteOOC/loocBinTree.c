/*
 ============================================================================
 Name        : loocBinTree.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 二叉树
 ============================================================================
 */

#include "loocBinTree.h"
#include <stdio.h>
#include <string.h>

/**
 * 初始化二叉树节点
 * @param cthis       当前二叉树节点对象指针
 * @param elementSize 二叉树元素大小
 * @param data        二叉树节点数据指针
 */
static void loocBinTreeNode_init(loocBinTreeNode* cthis, int elementSize,
		void* data) {
	cthis->_elementSize = elementSize;
	cthis->_data = looc_malloc(elementSize, "looBinTreeNode_Entry",
	looc_file_line);
	/* 将数据从栈空间拷贝到堆空间 */
	memcpy(cthis->_data, data, elementSize);
}

/**
 * 二叉树节点增加左节点
 * @param cthis   当前二叉树节点对象指针
 * @param newData 子节点二叉树数据指针
 */
static void loocBinTreeNode_setLeftChild(loocBinTreeNode* cthis, void* newData) {
	loocBinTreeNode* node = loocBinTreeNode_new(looc_file_line);
	node->init(node, cthis->_elementSize, newData);
	cthis->lChild = node;
	/* 增加引用计数 */
	node->loocObject._use++;
}

/**
 * 二叉树节点增加右节点
 * @param cthis   当前二叉树节点对象指针
 * @param newData 子节点二叉树数据指针
 */
static void loocBinTreeNode_setRightChild(loocBinTreeNode* cthis, void* newData) {
	loocBinTreeNode* node = loocBinTreeNode_new(looc_file_line);
	node->init(node, cthis->_elementSize, newData);
	cthis->rChild = node;
	/* 增加引用计数 */
	node->loocObject._use++;
}

/**
 * 二叉树节点销毁函数
 * @param object loocObject对象指针
 */
static void loocBinTreeNode_finalize(loocObject* object) {
	loocBinTreeNode* node = SUB_PTR(object, loocObject, loocBinTreeNode);
	if (node->_data) {
		looc_free(node->_data);
	}
	if (node->lChild) {
		if (node->lChild->loocObject._use) {
			node->lChild->loocObject._use--;
		}
		if (node->lChild->loocObject._use == 0) {
			loocBinTreeNode_delete(node->lChild);
		}
	}
	if (node->rChild) {
		if (node->rChild->loocObject._use) {
			node->rChild->loocObject._use--;
		}
		if (node->rChild->loocObject._use == 0) {
			loocBinTreeNode_delete(node->rChild);
		}
	}
}

/**
 * loocBinTreeNode构造函数
 */
CTOR(loocBinTreeNode)
/* 调用父类构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_data = NULL;
	cthis->_elementSize = 1;
	cthis->lChild = NULL;
	cthis->rChild = NULL;
	/* 成员函数绑定 */
	FUNCTION_SETTING(init, loocBinTreeNode_init);
	FUNCTION_SETTING(setLeftChild, loocBinTreeNode_setLeftChild);
	FUNCTION_SETTING(setRightChild, loocBinTreeNode_setRightChild);
	FUNCTION_SETTING(loocObject.finalize, loocBinTreeNode_finalize);END_CTOR

/**
 * loocBinTreeNode析构函数
 */
DTOR(loocBinTreeNode)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR

/**
 * 二叉树初始化
 * @param cthis       当前二叉树对象指针
 * @param elementSize 二叉树元素大小
 * @param pRoot       二叉树根节点指针
 */
static void loocBinTree_init(loocBinTree* cthis, int elementSize,
		loocBinTreeNode* pRoot) {
	cthis->_elementSize = elementSize;
	if (pRoot) {
		cthis->root = pRoot;
		/* 增加引用计数 */
		pRoot->loocObject._use++;
	}
}

/**
 * 前序遍历算法(递归法)
 * @param node    	二叉树节点对象指针
 * @param action 	对数据节点执行的操作，由用户自定义实现
 * @param args     	传递给action的第二个参数
 */
static void preOrder(loocBinTreeNode* node,
		void (*action)(loocBinTreeNode* node, void* args), void* args) {
	if (node != NULL) {
		action(node, args);
		preOrder(node->lChild, action, args);
		preOrder(node->rChild, action, args);
	}
}

/**
 * 中序遍历算法(递归法)
 * @param node    	二叉树节点对象指针
 * @param action 	对数据节点执行的操作，由用户自定义实现
 * @param args     	传递给action的第二个参数
 */
static void inOrder(loocBinTreeNode* node,
		void (*action)(loocBinTreeNode* node, void* args), void* args) {
	if (node != NULL) {
		inOrder(node->lChild, action, args);
		action(node, args);
		inOrder(node->rChild, action, args);
	}
}

/**
 * 后序遍历算法(递归法)
 * @param node    	二叉树节点对象指针
 * @param action 	对数据节点执行的操作，由用户自定义实现
 * @param args     	传递给action的第二个参数
 */
static void postOrder(loocBinTreeNode* node,
		void (*action)(loocBinTreeNode* node, void* args), void* args) {
	if (node != NULL) {
		postOrder(node->lChild, action, args);
		postOrder(node->rChild, action, args);
		action(node, args);
	}
}

/**
 * 二叉树的前序遍历
 * @param cthis    	当前二叉树对象指针
 * @param action 	对数据节点执行的操作，由用户自定义实现
 * @param args     	传递给action的第二个参数
 */
static void loocBinTree_preOrder(loocBinTree* cthis,
		void (*action)(loocBinTreeNode* node, void* args), void* args) {
	preOrder(cthis->root, action, args);
}

/**
 * 二叉树的中序遍历
 * @param cthis    	当前二叉树对象指针
 * @param action 	对数据节点执行的操作，由用户自定义实现
 * @param args     	传递给action的第二个参数
 */
static void loocBinTree_inOrder(loocBinTree* cthis,
		void (*action)(loocBinTreeNode* node, void* args), void* args) {
	inOrder(cthis->root, action, args);
}

/**
 * 二叉树的后序遍历
 * @param cthis    	当前二叉树对象指针
 * @param action 	对数据节点执行的操作，由用户自定义实现
 * @param args     	传递给action的第二个参数
 */
static void loocBinTree_postOrder(loocBinTree* cthis,
		void (*action)(loocBinTreeNode* node, void* args), void* args) {
	postOrder(cthis->root, action, args);
}

/**
 * 二叉树销毁函数
 * @param object loocObject对象指针
 */
static void loocBinTree_finalize(loocObject* object) {
	loocBinTree* tree = SUB_PTR(object, loocObject, loocBinTree);
	if (tree->root) {
		if (tree->root->loocObject._use) {
			tree->root->loocObject._use--;
		}
		if (tree->root->loocObject._use == 0) {
			loocBinTreeNode_delete(tree->root);
		}
	}
}

/**
 * loocBinTree构造函数
 */
CTOR(loocBinTree)
	SUPER_CTOR(loocObject);
	cthis->_elementSize = 1;
	cthis->root = NULL;
	FUNCTION_SETTING(init, loocBinTree_init);
	FUNCTION_SETTING(preOrder, loocBinTree_preOrder);
	FUNCTION_SETTING(inOrder, loocBinTree_inOrder);
	FUNCTION_SETTING(postOrder, loocBinTree_postOrder);
	FUNCTION_SETTING(loocObject.finalize, loocBinTree_finalize);END_CTOR

/**
 * loocBinTree析构函数
 */
DTOR(loocBinTree)
	SUPER_DTOR(loocObject);END_DTOR
