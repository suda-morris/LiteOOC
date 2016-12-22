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
#include "loocQueue.h"
#include "loocStack.h"
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
 * 二叉树的层序遍历
 * @param cthis    	当前二叉树对象指针
 * @param action 	对数据节点执行的操作，由用户自定义实现
 * @param args     	传递给action的第二个参数
 */
static void loocBinTree_layerOrder(loocBinTree* cthis,
		void (*action)(loocBinTreeNode* node, void* args), void* args) {
	loocBinTreeNode* node = NULL;
	/* 借助队列来实现层序遍历 */
	loocQueue* queue = NULL;
	if (cthis->root) {
		queue = loocQueue_new(looc_file_line);
		queue->init(queue, LOOC_QUEUE_SIZE_FOR_LAYER_ORDER, sizeof(node));
		queue->enqueue(queue, (void*) &(cthis->root));
		while (queue->length) {
			node = *(loocBinTreeNode**) (queue->dequeue(queue));
			action(node, args);
			if (node->lChild) {
				queue->enqueue(queue, (void*) &(node->lChild));
			}
			if (node->rChild) {
				queue->enqueue(queue, (void*) &(node->rChild));
			}
		}
		loocQueue_delete(queue);
	}
}

/**
 * 获取已知节点的父节点
 * @param  cthis 当前二叉树对象指针
 * @param  node  已知节点对象指针
 * @return       成功返回父节点对象指针，失败返回NULL
 */
static loocBinTreeNode* loocBinTree_getParent(loocBinTree* cthis,
		loocBinTreeNode* node) {
	loocQueue* queue = NULL;
	loocBinTreeNode* curNode = NULL;
	if (node == cthis->root) {
		return NULL;
	}
	if (node) {
		queue = loocQueue_new(looc_file_line);
		queue->init(queue, LOOC_QUEUE_SIZE_FOR_LAYER_ORDER, sizeof(node));
		queue->enqueue(queue, (void*) &(cthis->root));
		while (queue->length) {
			curNode = *(loocBinTreeNode**) queue->dequeue(queue);
			if (curNode->lChild) {
				if (curNode->lChild == node) {
					break;
				} else {
					queue->enqueue(queue, (void*) &(curNode->lChild));
				}
			}
			if (curNode->rChild) {
				if (curNode->rChild == node) {
					break;
				} else {
					queue->enqueue(queue, (void*) &(curNode->rChild));
				}
			}
		}
		loocQueue_delete(queue);
		if (curNode->lChild == node || curNode->rChild == node) {
			return curNode;
		}
	}
	return NULL;
}

/**
 * 获取已知节点的兄弟节点
 * @param  cthis 当前二叉树对象指针
 * @param  node  已知节点对象指针
 * @return       成功返回兄弟节点对象指针，失败返回NULL
 */
static loocBinTreeNode* loocBinTree_getBrother(loocBinTree* cthis,
		loocBinTreeNode* node) {
	loocBinTreeNode* parent;
	if (node) {
		parent = cthis->getParent(cthis, node);
		if (parent) {
			if (parent->lChild == node) {
				return parent->rChild;
			}
			if (parent->rChild == node) {
				return parent->lChild;
			}
		}
	}
	return NULL;
}

/**
 * 获取树的高度（广度优先，层序遍历）
 * @param  cthis 当前树对象指针
 * @return       返回树的高度
 */
static int loocBinTree_getHeight(loocBinTree* cthis) {
	int maxHeight = 0;
	int curHeight = 0;
	int tempHeight = 0;
	loocQueue* nodeQueue;
	loocQueue* heightQueue;
	loocBinTreeNode* node = cthis->root;
	if (node) {
		/* 因为根节点存在，初始化当前高度为1 */
		curHeight = 1;
		/* 保存节点的队列 */
		nodeQueue = loocQueue_new(looc_file_line);
		nodeQueue->init(nodeQueue, LOOC_QUEUE_SIZE_FOR_LAYER_ORDER,
				sizeof(node));
		/* 保存高度的队列 */
		heightQueue = loocQueue_new(looc_file_line);
		heightQueue->init(heightQueue, LOOC_QUEUE_SIZE_FOR_LAYER_ORDER,
				sizeof(int));
		nodeQueue->enqueue(nodeQueue, (void*) &node);
		heightQueue->enqueue(heightQueue, (void*) &curHeight);
		while (nodeQueue->length) {
			/* 出队 */
			node = *(loocBinTreeNode**) (nodeQueue->dequeue(nodeQueue));
			curHeight = *(int*) (heightQueue->dequeue(heightQueue));
			/* 更新maxHeight */
			if (curHeight > maxHeight) {
				maxHeight = curHeight;
			}
			/* 将左右节点入队 */
			if (node->lChild) {
				nodeQueue->enqueue(nodeQueue, (void*) &(node->lChild));
				tempHeight = curHeight + 1;
				heightQueue->enqueue(heightQueue, (void*) &tempHeight);
			}
			if (node->rChild) {
				nodeQueue->enqueue(nodeQueue, (void*) &(node->rChild));
				tempHeight = curHeight + 1;
				heightQueue->enqueue(heightQueue, (void*) &tempHeight);
			}
		}
		/* 释放队列的内存 */
		loocQueue_delete(nodeQueue);
		loocQueue_delete(heightQueue);
	}
	return maxHeight;
}

/**
 * 获取指定节点的深度（深度优先，后序遍历）
 * @param  cthis 当前二叉树对象指针
 * @param  node  指定节点对象指针
 * @return       返回指定节点的深度
 */
static int loocBinTree_getDepthOfNode(loocBinTree* cthis, loocBinTreeNode* node) {
	int depth = 0;
	loocStack* nodeStack = NULL;
	loocStack* flagStack = NULL;
	/* flag表示每个节点的访问状态，1表示访问一次，2表示访问两次 */
	int curFlag = 0;
	loocBinTreeNode* curNode = cthis->root;
	if (curNode) {
		/* 存储二叉树节点的栈 */
		nodeStack = loocStack_new(looc_file_line);
		nodeStack->init(nodeStack, LOOC_STACK_SIZE_FOR_DEPTH_ORDER,
				sizeof(curNode));
		/* 存储每个节点的状态的栈 */
		flagStack = loocStack_new(looc_file_line);
		flagStack->init(flagStack, LOOC_STACK_SIZE_FOR_DEPTH_ORDER,
				sizeof(int));
		do {
			while (curNode) {
				nodeStack->push(nodeStack, (void*) &curNode);
				curFlag = 1;
				flagStack->push(flagStack, (void*) &curFlag);
				curNode = curNode->lChild;
			}
			/* 一条路径走到头，出栈栈顶元素尝试新路径 */
			curNode = *(loocBinTreeNode**) (nodeStack->pop(nodeStack));
			curFlag = *(int*) flagStack->pop(flagStack);
			/* 将第一次访问的节点再入栈 */
			if (curFlag == 1) {
				curFlag = 2;
				nodeStack->push(nodeStack, (void*) &curNode);
				flagStack->push(flagStack, (void*) &curFlag);
				curNode = curNode->rChild;
			} else {
				/* 左右节点均以遍历，轮到根节点 */
				if (curNode == node) {
					depth = nodeStack->top + 2;
					break;
				}
				curNode = NULL;
			}
		} while (curNode || nodeStack->top != -1);
		/* 释放栈的内存 */
		loocStack_delete(nodeStack);
		loocStack_delete(flagStack);
	}
	return depth;
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
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_elementSize = 1;
	cthis->root = NULL;
	/* 成员函数绑定 */
	FUNCTION_SETTING(init, loocBinTree_init);
	FUNCTION_SETTING(preOrder, loocBinTree_preOrder);
	FUNCTION_SETTING(inOrder, loocBinTree_inOrder);
	FUNCTION_SETTING(postOrder, loocBinTree_postOrder);
	FUNCTION_SETTING(layerOrder, loocBinTree_layerOrder);
	FUNCTION_SETTING(getParent, loocBinTree_getParent);
	FUNCTION_SETTING(getBrother, loocBinTree_getBrother);
	FUNCTION_SETTING(getHeight, loocBinTree_getHeight);
	FUNCTION_SETTING(getDepthOfNode, loocBinTree_getDepthOfNode);
	FUNCTION_SETTING(loocObject.finalize, loocBinTree_finalize);END_CTOR

/**
 * loocBinTree析构函数
 */
DTOR(loocBinTree)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
