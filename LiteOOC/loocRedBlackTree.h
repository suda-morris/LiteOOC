/*
 ============================================================================
 Name        : loocRedBlackTree.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 红黑树
 ============================================================================
 */

#ifndef LITEOOC_LOOCREDBLACKTREE_H_
#define LITEOOC_LOOCREDBLACKTREE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "looc.h"
#include "loocObject.h"

typedef enum looc_RBT_Color_e {
	looc_RBT_Black = 0, looc_RBT_Red = 1
} looc_RBT_Color;
/**
 * 红黑树节点具体类
 */
CLASS(loocRedBlackTreeNode) {
	/* 继承自loocObject类  */
	EXTENDS(loocObject);
	/* 数据域，指向实际的数据 */
	void* _data;
	/* 颜色域，0表示黑，1表示红 */
	looc_RBT_Color color;
	/* 红黑树中的元素大小 */
	int _elementSize;
	/* 指向左右节点和父节点 */
	loocRedBlackTreeNode* lChild;
	loocRedBlackTreeNode* rChild;
	loocRedBlackTreeNode* parent;
	/* 初始化一个红黑树节点*/
	void (*init)(loocRedBlackTreeNode* cthis, int elementSize, void* data,
			looc_RBT_Color color);
};

/**
 * 红黑树具体类
 */
CLASS(loocRedBlackTree) {
	/* 继承自loocObject类  */
	EXTENDS(loocObject);
	/* 红黑树中的元素大小 */
	int _elementSize;
	/* 红黑树节点个数 */
	int length;
	/* 每个红黑树都会有一个根节点*/
	loocRedBlackTreeNode* root;
	/* 哨兵叶子节点 */
	loocRedBlackTreeNode* nil;
	/* 初始化一个红黑树 */
	void (*init)(loocRedBlackTree* cthis, int elementSize,
			loocRedBlackTreeNode* pRoot,
			int (*compareStrategy)(loocRedBlackTreeNode* cthis,
					loocRedBlackTreeNode* node));
};

#ifdef __cplusPlus
}
#endif

#endif /* LITEOOC_LOOCREDBLACKTREE_H_ */
