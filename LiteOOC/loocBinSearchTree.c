/*
 ============================================================================
 Name        : loocBinSearchTree.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 二叉查找树
 ============================================================================
 */

#include <loocBinSearchTree.h>
#include <string.h>

/**
 * 如果node比cthis小，则返回-1，相等则返回0，大于则返回1
 */
static int (*compare)(loocBinSearchTreeNode* cthis, loocBinSearchTreeNode* node) =
NULL;

/**
 * 初始化二叉查找树节点
 * @param cthis       当前二叉查找树节点对象指针
 * @param elementSize 二叉查找树元素大小
 * @param data        二叉查找树节点数据指针
 */
static void loocBinSearchTreeNode_init(loocBinSearchTreeNode* cthis,
		int elementSize, void* data) {
	cthis->_elementSize = elementSize;
	cthis->_data = looc_malloc(elementSize, "loocBinSearchTreeNode_Entry",
	looc_file_line);
	/* 将数据从栈空间拷贝到堆空间 */
	memcpy(cthis->_data, data, elementSize);
}

/**
 * 二叉查找树节点销毁函数
 * @param object loocObject对象指针
 */
static void loocBinSearchTreeNode_finalize(loocObject* object) {
	loocBinSearchTreeNode* node = SUB_PTR(object, loocObject,
			loocBinSearchTreeNode);
	if (node->_data) {
		looc_free(node->_data);
	}
	if (node->lChild) {
		if (node->lChild->loocObject._use) {
			node->lChild->loocObject._use--;
		}
		if (node->lChild->loocObject._use == 0) {
			loocBinSearchTreeNode_delete(node->lChild);
		}
	}
	if (node->rChild) {
		if (node->rChild->loocObject._use) {
			node->rChild->loocObject._use--;
		}
		if (node->rChild->loocObject._use == 0) {
			loocBinSearchTreeNode_delete(node->rChild);
		}
	}
}

/**
 * loocBinSearchTreeNode的构造函数
 */
CTOR(loocBinSearchTreeNode)
/* 调用父类构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_data = NULL;
	cthis->_elementSize = 1;
	cthis->lChild = NULL;
	cthis->rChild = NULL;
	cthis->parent = NULL;
	/* 成员函数绑定 */
	FUNCTION_SETTING(init, loocBinSearchTreeNode_init);
	FUNCTION_SETTING(loocObject.finalize, loocBinSearchTreeNode_finalize);END_CTOR

/**
 * loocBinSearchTreeNode的析构函数
 */
DTOR(loocBinSearchTreeNode)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR

/**
 * 二叉查找树初始化
 * @param cthis       当前二叉查找树对象指针
 * @param elementSize 二叉查找树元素大小
 * @param pRoot       二叉查找树根节点指针
 * @param compare     二叉查找树的比较策略,由用户去定义
 */
static void loocBinSearchTree_init(loocBinSearchTree* cthis, int elementSize,
		loocBinSearchTreeNode* pRoot,
		int (*compareStrategy)(loocBinSearchTreeNode* cthis,
				loocBinSearchTreeNode* node)) {
	compare = compareStrategy;
	cthis->_elementSize = elementSize;
	if (pRoot) {
		cthis->root = pRoot;
		cthis->length++;
	}
}

/**
 * 二叉查找树插入操作
 * @param cthis 当前二叉查找树对象指针
 * @param data  待插入的数据指针
 * @return      成功返回true，失败返回false
 */
static looc_bool loocBinSearchTree_insert(loocBinSearchTree* cthis, void* data) {
	/* 检查是否定义了compare策略 */
	if (compare == NULL) {
		return looc_false;
	}
	loocBinSearchTreeNode* curNode = cthis->root;
	loocBinSearchTreeNode* tempNode = NULL;
	loocBinSearchTreeNode* node = loocBinSearchTreeNode_new(looc_file_line);
	node->init(node, cthis->_elementSize, data);
	while (curNode) {
		tempNode = curNode;
		/* 待插入节点比当前节点的值小则遍历左子树 */
		if (compare(curNode, node) < 0) {
			curNode = curNode->lChild;
		} else {
			curNode = curNode->rChild;
		}
	}
	node->parent = tempNode;
	if (tempNode == NULL) {
		cthis->root = node;
	} else {
		if (compare(tempNode, node) < 0) {
			tempNode->lChild = node;
		} else {
			tempNode->rChild = node;
		}
		/* 增加引用计数 */
		node->loocObject._use++;
	}
	cthis->length++;
	return looc_true;
}

/**
 * 中序遍历算法(递归法)
 * @param node    	二叉查找树节点对象指针
 * @param action 	对数据节点执行的操作，由用户自定义实现
 * @param args     	传递给action的第二个参数
 */
static void inOrder(loocBinSearchTreeNode* node,
		void (*action)(loocBinSearchTreeNode* node, void* args), void* args) {
	if (node != NULL) {
		inOrder(node->lChild, action, args);
		action(node, args);
		inOrder(node->rChild, action, args);
	}
}

/**
 * 二叉查找树的中序遍历（即排序）
 * @param cthis    	当前二叉查找树对象指针
 * @param action 	对数据节点执行的操作，由用户自定义实现
 * @param args     	传递给action的第二个参数
 */
static void loocBinSearchTree_inOrder(loocBinSearchTree* cthis,
		void (*action)(loocBinSearchTreeNode* node, void* args), void* args) {
	inOrder(cthis->root, action, args);
}

/**
 * 二叉查找树查询操作
 * @param  cthis 当前二叉查找树对象指针
 * @param  data  待查询的数据指针
 * @return       返回查询到的节点指针
 */
static loocBinSearchTreeNode* loocBinSearchTree_search(loocBinSearchTree* cthis,
		void* data) {
	loocBinSearchTreeNode* curNode = cthis->root;
	loocBinSearchTreeNode* node;
	if (curNode) {
		/* 重新构造一个节点 */
		node = loocBinSearchTreeNode_new(looc_file_line);
		node->init(node, cthis->_elementSize, data);
		while (curNode) {
			if (compare(curNode, node) < 0) {
				curNode = curNode->lChild;
			} else if (compare(curNode, node) > 0) {
				curNode = curNode->rChild;
			} else {
				break;
			}
		}
		/* 释放临时节点的内存空间 */
		loocBinSearchTreeNode_delete(node);
	}
	return curNode;
}

/**
 * 获取二叉查找树中的最大的节点
 * @param  cthis 当前二叉查找树
 * @return       最大的二叉查找树节点
 */
static loocBinSearchTreeNode* loocBinSearchTree_getMaxNode(
		loocBinSearchTree* cthis) {
	loocBinSearchTreeNode* node;
	loocBinSearchTreeNode* temp = cthis->root;
	while (temp) {
		node = temp;
		temp = temp->rChild;
	}
	return node;
}

/**
 * 获取二叉查找树中的最小的节点
 * @param  cthis 当前二叉查找树
 * @return       最小的二叉查找树节点
 */
static loocBinSearchTreeNode* loocBinSearchTree_getMinNode(
		loocBinSearchTree* cthis) {
	loocBinSearchTreeNode* node;
	loocBinSearchTreeNode* temp = cthis->root;
	while (temp) {
		node = temp;
		temp = temp->lChild;
	}
	return node;
}

/**
 * 二叉查找树删除节点的操作
 * @param  cthis 当前二叉查找树对象指针
 * @param  node  欲删除节点的指针
 * @return       成功返回true，失败返回false
 */
static looc_bool loocBinSearchTree_deleteNode(loocBinSearchTree* cthis,
		loocBinSearchTreeNode* node) {
	loocBinSearchTreeNode* y;
	loocBinSearchTreeNode* x;
	if (node == NULL) {
		return looc_false;
	}
	/* node有一个子节点或者没有子节点 */
	if (node->lChild == NULL || node->rChild == NULL) {
		y = node;
	}
	/* node有两个子节点 */
	else {
		y = node->rChild;
		while (y->lChild) {
			y = y->lChild;
		}
	}
	if (y->lChild) {
		x = y->lChild;
	} else {
		x = y->rChild;
	}
	if (x) {
		x->parent = y->parent;
	}
	if (y->parent == NULL) {
		cthis->root = x;
		if (x) {
			x->loocObject._use--;
		}
	} else if (y->parent->lChild == y) {
		y->parent->lChild = x;
	} else if (y->parent->rChild == y) {
		y->parent->rChild = x;
	}
	if (y != node) {
		memcpy(node->_data, y->_data, cthis->_elementSize);
	}
	y->lChild = NULL;
	y->rChild = NULL;
	loocBinSearchTreeNode_delete(y);
	cthis->length--;
	return looc_true;
}

/**
 * 二叉查找树销毁函数
 * @param object loocObject对象指针
 */
static void loocBinSearchTree_finalize(loocObject* object) {
	loocBinSearchTree* tree = SUB_PTR(object, loocObject, loocBinSearchTree);
	if (tree->root) {
		if (tree->root->loocObject._use) {
			tree->root->loocObject._use--;
		}
		if (tree->root->loocObject._use == 0) {
			loocBinSearchTreeNode_delete(tree->root);
		}
	}
}

/**
 * loocBinSearchTree构造函数
 */
CTOR(loocBinSearchTree)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_elementSize = 1;
	cthis->root = NULL;
	cthis->length = 0;
	/* 成员函数绑定 */
	FUNCTION_SETTING(init, loocBinSearchTree_init);
	FUNCTION_SETTING(insert, loocBinSearchTree_insert);
	FUNCTION_SETTING(inOrder, loocBinSearchTree_inOrder);
	FUNCTION_SETTING(search, loocBinSearchTree_search);
	FUNCTION_SETTING(getMaxNode, loocBinSearchTree_getMaxNode);
	FUNCTION_SETTING(getMinNode, loocBinSearchTree_getMinNode);
	FUNCTION_SETTING(deleteNode, loocBinSearchTree_deleteNode);
	FUNCTION_SETTING(loocObject.finalize, loocBinSearchTree_finalize);END_CTOR

/**
 * loocBinSearchTree析构函数
 */
DTOR(loocBinSearchTree)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
