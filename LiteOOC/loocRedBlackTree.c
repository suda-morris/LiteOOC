/*
 ============================================================================
 Name        : loocRedBlackTree.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 红黑树
 ============================================================================
 */

#include <loocRedBlackTree.h>
#include <string.h>

/**
 * 如果node比cthis小，则返回-1，相等则返回0，大于则返回1
 */
static int (*compare)(loocRedBlackTreeNode* cthis, loocRedBlackTreeNode* node) =
NULL;

/**
 * 初始化红黑树节点
 * @param cthis       当前红黑树节点对象指针
 * @param elementSize 红黑树元素大小
 * @param data        红黑树节点数据指针
 * @param color       红黑树节点颜色
 */
static void loocRedBlackTreeNode_init(loocRedBlackTreeNode* cthis,
		int elementSize, void* data, looc_RBT_Color color) {
	cthis->_elementSize = elementSize;
	cthis->color = color;
	cthis->_data = looc_malloc(elementSize, "loocRedBlackTreeNode_Entry",
	looc_file_line);
	/* 将数据从栈空间拷贝到堆空间 */
	memcpy(cthis->_data, data, elementSize);
}

/**
 * 红黑树节点销毁函数
 * @param object loocObject对象指针
 */
static void loocRedBlackTreeNode_finalize(loocObject* object) {
	loocRedBlackTreeNode* node = SUB_PTR(object, loocObject,
			loocRedBlackTreeNode);
	if (node->_data) {
		looc_free(node->_data);
	}
	if (node->lChild) {
		if (node->lChild->loocObject._use) {
			node->lChild->loocObject._use--;
		}
		if (node->lChild->loocObject._use == 0) {
			loocRedBlackTreeNode_delete(node->lChild);
		}
	}
	if (node->rChild) {
		if (node->rChild->loocObject._use) {
			node->rChild->loocObject._use--;
		}
		if (node->rChild->loocObject._use == 0) {
			loocRedBlackTreeNode_delete(node->rChild);
		}
	}
}

/**
 * loocRedBlackTreeNode的构造函数
 */
CTOR(loocRedBlackTreeNode)
/* 调用父类构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_data = NULL;
	cthis->_elementSize = 1;
	cthis->color = looc_RBT_Black;
	cthis->lChild = NULL;
	cthis->rChild = NULL;
	cthis->parent = NULL;
	/* 成员函数绑定 */
	FUNCTION_SETTING(init, loocRedBlackTreeNode_init);
	FUNCTION_SETTING(loocObject.finalize, loocRedBlackTreeNode_finalize);END_CTOR

/**
 * loocRedBlackTreeNode的析构函数
 */
DTOR(loocRedBlackTreeNode)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR

/**
 * 红黑树初始化
 * @param cthis       当前红黑树对象指针
 * @param elementSize 红黑树元素大小
 * @param pRoot       红黑树根节点指针
 * @param compare     红黑树的比较策略,由用户去定义
 */
static void loocRedBlackTree_init(loocRedBlackTree* cthis, int elementSize,
		loocRedBlackTreeNode* pRoot,
		int (*compareStrategy)(loocRedBlackTreeNode* cthis,
				loocRedBlackTreeNode* node)) {
	compare = compareStrategy;
	cthis->_elementSize = elementSize;
	cthis->nil = loocRedBlackTreeNode_new(looc_file_line);
	if (pRoot) {
		cthis->root = pRoot;
		cthis->root->color = looc_RBT_Black;	//根节点为黑色
		cthis->length++;
	}
}

/**
 * 红黑树销毁函数
 * @param object loocObject对象指针
 */
static void loocRedBlackTree_finalize(loocObject* object) {
	loocRedBlackTree* tree = SUB_PTR(object, loocObject, loocRedBlackTree);
	if (tree->root) {
		if (tree->root->loocObject._use) {
			tree->root->loocObject._use--;
		}
		if (tree->root->loocObject._use == 0) {
			loocRedBlackTreeNode_delete(tree->root);
		}
	}
	if (tree->nil) {
		if (tree->nil->loocObject._use) {
			tree->nil->loocObject._use--;
		}
		if (tree->nil->loocObject._use == 0) {
			loocRedBlackTreeNode_delete(tree->nil);
		}
	}
}

/**
 * loocRedBlackTree构造函数
 */
CTOR(loocRedBlackTree)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_elementSize = 1;
	cthis->root = NULL;
	cthis->nil = NULL;
	cthis->length = 0;
	/* 成员函数绑定 */
	FUNCTION_SETTING(init, loocRedBlackTree_init);
	FUNCTION_SETTING(loocObject.finalize, loocRedBlackTree_finalize);END_CTOR

/**
 * loocRedBlackTree析构函数
 */
DTOR(loocRedBlackTree)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
