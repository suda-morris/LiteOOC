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
	cthis->color = looc_RBT_Red;	//红黑树默认节点颜色为红色
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
	cthis->nil->color = looc_RBT_Black;	//nil节点为黑色
	if (pRoot) {
		cthis->root = pRoot;
		cthis->root->color = looc_RBT_Black;	//根节点为黑色
		cthis->root->lChild = cthis->nil;
		cthis->root->rChild = cthis->nil;
		cthis->root->parent = cthis->nil;
		cthis->length++;
	} else {
		cthis->root = cthis->nil;
	}
}

/**
 * 红黑树左旋转操作
 * @param  cthis 当前红黑树对象指针
 * @param  node  需要旋转的节点指针(局部根节点)
 * @return       成功返回true，失败返回false
 */
static looc_bool loocRedBlackTree_leftRotated(loocRedBlackTree* cthis,
		loocRedBlackTreeNode* node) {
	loocRedBlackTreeNode* y = NULL;
	if (node == NULL) {
		return looc_false;
	}
	if (node->rChild != cthis->nil) {
		y = node->rChild;
		node->rChild = y->lChild;
		if (y->lChild != cthis->nil) {
			y->lChild->parent = node;
		}
		y->parent = node->parent;
		if (node->parent == cthis->nil) {
			node->loocObject._use++;
			y->loocObject._use--;
			cthis->root = y;
		} else if (node->parent->lChild == node) {
			node->parent->lChild = y;
		} else {
			node->parent->rChild = y;
		}
		y->lChild = node;
		node->parent = y;
		return looc_true;
	} else {
		return looc_false;
	}
}

/**
 * 红黑树右旋转操作
 * @param  cthis 当前红黑树对象指针
 * @param  node  需要旋转的节点指针(局部根节点)
 * @return       成功返回true，失败返回false
 */
static looc_bool loocRedBlackTree_rightRotated(loocRedBlackTree* cthis,
		loocRedBlackTreeNode* node) {
	loocRedBlackTreeNode* y = NULL;
	if (node == NULL) {
		return looc_false;
	}
	if (node->lChild != cthis->nil) {
		y = node->lChild;
		node->lChild = y->rChild;
		if (y->rChild != cthis->nil) {
			y->rChild->parent = node;
		}
		y->parent = node->parent;
		if (node->parent == cthis->nil) {
			node->loocObject._use++;
			y->loocObject._use--;
			cthis->root = y;
		} else if (node->parent->rChild == node) {
			node->parent->rChild = y;
		} else {
			node->parent->lChild = y;
		}
		y->rChild = node;
		node->parent = y;
		return looc_true;
	} else {
		return looc_false;
	}
}

/**
 * 插入操作结束后恢复红黑树的性质
 * @param tree 红黑树对象指针
 * @param node 被插入节点指针
 */
static void insertKeepRB(loocRedBlackTree* cthis, loocRedBlackTreeNode* node) {
	/* 叔节点 */
	loocRedBlackTreeNode* y = NULL;
	/* 节点向上递归更改 */
	while (node->parent->color == looc_RBT_Red) {
		/* node节点的父节点是左孩子的情况 */
		if (node->parent->parent->lChild == node->parent) {
			y = node->parent->parent->rChild;
			/* 叔节点为红色 */
			if (y->color == looc_RBT_Red) {
				node->parent->color = looc_RBT_Black;	//更改父节点的颜色为黑色
				y->color = looc_RBT_Black;	//更改叔节点的颜色为黑色
				node->parent->parent->color = looc_RBT_Red;	//更改祖节点的颜色为红色
				/* 指向祖节点递归向上 */
				node = node->parent->parent;
			}
			/* 叔节点为黑色，node是父节点的左孩子 */
			else if (node->parent->lChild == node) {
				node->parent->color = looc_RBT_Black;	//更改父节点为黑色
				node->parent->parent->color = looc_RBT_Red;	//更改祖节点的颜色为红色
				/* 右旋祖节点 */
				loocRedBlackTree_rightRotated(cthis, node->parent->parent);	//右旋祖节点
			}
			/* 叔节点是黑色，node是父节点的右孩子 */
			else if (node->parent->rChild == node) {
				node = node->parent;
				loocRedBlackTree_leftRotated(cthis, node);	//先左旋父节点，变成上一种情况
				node->parent->color = looc_RBT_Black;	//更改父节点的颜色为黑色
				node->parent->parent->color = looc_RBT_Red;	//更改祖节点的颜色为红色
				loocRedBlackTree_rightRotated(cthis, node->parent->parent);	//右旋祖节点
			}
		}
		/* node节点的父节点是右孩子的情况，与上面左右对称 */
		else {
			y = node->parent->parent->lChild;
			if (y->color == looc_RBT_Red) {
				node->parent->color = looc_RBT_Black;
				y->color = looc_RBT_Black;
				node->parent->parent->color = looc_RBT_Red;
				node = node->parent->parent;
			} else if (node->parent->rChild == node) {
				node->parent->color = looc_RBT_Black;
				node->parent->parent->color = looc_RBT_Red;
				loocRedBlackTree_leftRotated(cthis, node->parent->parent);
			} else if (node->parent->lChild == node) {
				node = node->parent;
				loocRedBlackTree_rightRotated(cthis, node);
				node->parent->color = looc_RBT_Black;
				node->parent->parent->color = looc_RBT_Red;
				loocRedBlackTree_leftRotated(cthis, node->parent->parent);
			}
		}
	}
	cthis->root->color = looc_RBT_Black;	//将根节点设置为黑色，可能导致所有路径黑高度都加1
}

/**
 * 红黑树的插入操作
 * @param  cthis 当前红黑树对象指针
 * @param  node  待插入的节点指针
 * @return       成功返回true，失败返回false
 */
static looc_bool loocRedBlackTree_insert(loocRedBlackTree* cthis, void* data) {
	if (compare == NULL) {
		return looc_false;
	}
	loocRedBlackTreeNode* curNode = cthis->root;
	loocRedBlackTreeNode* tempNode = cthis->nil;
	loocRedBlackTreeNode* node = loocRedBlackTreeNode_new(looc_file_line);
	/* 新插入的节点默认设置为红色 */
	node->init(node, cthis->_elementSize, data, looc_RBT_Red);
	while (curNode != cthis->nil) {
		tempNode = curNode;
		if (compare(curNode, node) < 0) {
			curNode = curNode->lChild;
		} else {
			curNode = curNode->rChild;
		}
	}
	node->parent = tempNode;	//最后一个有效节点设置为待插入节点的父节点
	if (tempNode == cthis->nil) {
		cthis->root = node;		//如果根节点为空，待插入节点作为根节点
	} else {
		if (compare(tempNode, node) < 0) {
			tempNode->lChild = node;
		} else {
			tempNode->rChild = node;
		}
		/* 增加引用计数 */
		node->loocObject._use++;
	}
	node->lChild = cthis->nil;	//让新节点的左右孩子指向nil节点
	node->rChild = cthis->nil;
	cthis->length++;
	insertKeepRB(cthis, node);	//恢复红黑树的性质
	return looc_true;
}

/**
 * 删除节点操作结束后恢复红黑树的性质
 * @param tree 红黑树对象指针
 * @param node 被删除节点指针
 */
static void deleteKeepRB(loocRedBlackTree* cthis, loocRedBlackTreeNode* x) {
	/* 兄弟节点指针 */
	loocRedBlackTreeNode* w = NULL;
	/* x非黑或者为根节点很好处理，主要是x为黑节点的时候，需要进行大规模调整 */
	while (x != cthis->root && x->color == looc_RBT_Black) {
		/* x为左孩子 */
		if (x == x->parent->lChild) {
			w = x->parent->rChild;
			/* 兄弟节点为红色 */
			if (w->color == looc_RBT_Red) {
				w->color = looc_RBT_Black;
				x->parent->color = looc_RBT_Red;
				loocRedBlackTree_leftRotated(cthis, x->parent);
				w = x->parent->rChild;
			}
			/* 兄弟节点是黑色，且兄弟节点的两个子节点也是黑色 */
			if (w->lChild->color == looc_RBT_Black
					&& w->rChild->color == looc_RBT_Black) {
				w->color = looc_RBT_Red;
				x = x->parent;	//递归向上调整
			}
			/* 兄弟节点是黑色，兄弟节点的左孩子是红色，右孩子是黑色 */
			else if (w->rChild->color == looc_RBT_Black) {
				w->lChild->color = looc_RBT_Black;
				w->color = looc_RBT_Red;
				loocRedBlackTree_rightRotated(cthis, w);	//兄弟节点右旋
				w = x->parent->rChild;	//这样兄弟节点的右孩子变成了红色
			}
			/* 兄弟节点是黑色，兄弟节点的右孩子是红色 */
			else {
				w->color = x->parent->color;
				x->parent->color = looc_RBT_Black;
				x->rChild->color = looc_RBT_Black;
				loocRedBlackTree_leftRotated(cthis, x->parent);	//左旋父节点
				x = cthis->root;
			}
		}
		/* x为右孩子,与左孩子对称 */
		else {
			w = x->parent->lChild;
			if (w->color == looc_RBT_Red) {
				w->color = looc_RBT_Black;
				x->parent->color = looc_RBT_Red;
				loocRedBlackTree_rightRotated(cthis, x->parent);
				w = x->parent->lChild;
			}
			if (w->lChild->color == looc_RBT_Black
					&& w->rChild->color == looc_RBT_Black) {
				w->color = looc_RBT_Red;
				x = x->parent;
			} else if (w->lChild->color == looc_RBT_Black) {
				w->rChild->color = looc_RBT_Black;
				w->color = looc_RBT_Red;
				loocRedBlackTree_leftRotated(cthis, w);
				w = x->parent->lChild;
			} else {
				w->color = x->parent->color;
				x->parent->color = looc_RBT_Black;
				x->lChild->color = looc_RBT_Black;
				loocRedBlackTree_rightRotated(cthis, x->parent);
				x = cthis->root;
			}
		}
	}
	/* 确保x为黑色 */
	x->color = looc_RBT_Black;
}

/**
 * 红黑树删除节点操作
 * @param  cthis 当前红黑树对象指针
 * @param  node  被删除结点指针
 * @return       成功返回true，失败返回false
 */
static looc_bool loocRedBlackTree_deleteNode(loocRedBlackTree* cthis,
		loocRedBlackTreeNode* node) {
	/* y指向替身节点（实际会被删除的节点） */
	loocRedBlackTreeNode* y;
	/* x指向替身节点的继承者 */
	loocRedBlackTreeNode* x;
	if (node == NULL) {
		return looc_false;
	}
	/* node只有一个孩子或者没有孩子 */
	if (node->lChild == cthis->nil || node->rChild == cthis->nil) {
		y = node;
	}
	/* node有两个孩子，y指向node右子树的最小节点*/
	else {
		y = node->rChild;
		while (y->lChild != cthis->nil) {
			y = y->lChild;
		}
	}
	/* x为y的左右子树之一，或者为nil节点 */
	if (y->lChild != cthis->nil) {
		x = y->lChild;
	} else {
		x = y->rChild;
	}
	/* 设置x的父节点 */
	if (x != cthis->nil) {
		x->parent = y->parent;
	}
	/* 只有1个或者2个节点的时候y的父节点才为空 */
	if (y->parent == cthis->nil) {
		cthis->root = x;
		if (x != cthis->nil) {
			/* 从普通节点变为根节点，引用计数减一 */
			x->loocObject._use--;
		}
	}
	/* 确定x在y的父节点的位置 */
	else if (y == y->parent->lChild) {
		y->parent->lChild = x;
	} else {
		y->parent->rChild = x;
	}
	/* 将y的数据和颜色复制到node的节点中，这就是y作为替身节点的作用 */
	if (y != node) {
		node->color = y->color;
		memcpy(node->_data, y->_data, cthis->_elementSize);
	}
	/* 如果删除了黑节点势必会破坏红黑树的性质，需要做调整 */
	if (x != cthis->nil && y->color == looc_RBT_Black) {
		deleteKeepRB(cthis, x);
	}
	/* 删除替身节点 */
	y->lChild = NULL;
	y->rChild = NULL;
	loocRedBlackTreeNode_delete(y);
	cthis->length--;
	return looc_true;
}

/**
 * 中序遍历算法(递归法)
 * @param node    	红黑树节点对象指针
 * @param action 	对数据节点执行的操作，由用户自定义实现
 * @param args     	传递给action的第二个参数
 */
static void inOrder(loocRedBlackTree* cthis, loocRedBlackTreeNode* node,
		void (*action)(loocRedBlackTreeNode* node, void* args), void* args) {
	if (node != cthis->nil) {
		inOrder(cthis, node->lChild, action, args);
		action(node, args);
		inOrder(cthis, node->rChild, action, args);
	}
}

/**
 * 红黑树的中序遍历（即排序）
 * @param cthis    	当前红黑树对象指针
 * @param action 	对数据节点执行的操作，由用户自定义实现
 * @param args     	传递给action的第二个参数
 */
static void loocRedBlackTree_inOrder(loocRedBlackTree* cthis,
		void (*action)(loocRedBlackTreeNode* node, void* args), void* args) {
	inOrder(cthis, cthis->root, action, args);
}

/**
 * 红黑树查询操作
 * @param  cthis 当前红黑树对象指针
 * @param  data  待查询的数据指针
 * @return       返回查询到的节点指针
 */
static loocRedBlackTreeNode* loocBinSearchTree_search(loocRedBlackTree* cthis,
		void* data) {
	loocRedBlackTreeNode* curNode = cthis->root;
	loocRedBlackTreeNode* node;
	if (curNode) {
		/* 重新构造一个节点 */
		node = loocRedBlackTreeNode_new(looc_file_line);
		node->init(node, cthis->_elementSize, data, 0);
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
		loocRedBlackTreeNode_delete(node);
	}
	return curNode;
}

/**
 * 红黑树销毁函数
 * @param object loocObject对象指针
 */
static void loocRedBlackTree_finalize(loocObject* object) {
	loocRedBlackTree* tree = SUB_PTR(object, loocObject, loocRedBlackTree);
	if (tree->nil) {
		if (tree->nil->loocObject._use) {
			tree->nil->loocObject._use--;
		}
		if (tree->nil->loocObject._use == 0) {
			loocRedBlackTreeNode_delete(tree->nil);
		}
	}
	if (tree->root) {
		if (tree->root->loocObject._use) {
			tree->root->loocObject._use--;
		}
		if (tree->root->loocObject._use == 0) {
			loocRedBlackTreeNode_delete(tree->root);
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
	FUNCTION_SETTING(leftRotated, loocRedBlackTree_leftRotated);
	FUNCTION_SETTING(rightRotated, loocRedBlackTree_rightRotated);
	FUNCTION_SETTING(insert, loocRedBlackTree_insert);
	FUNCTION_SETTING(deleteNode, loocRedBlackTree_deleteNode);
	FUNCTION_SETTING(inOrder, loocRedBlackTree_inOrder);
	FUNCTION_SETTING(search, loocBinSearchTree_search);
	FUNCTION_SETTING(loocObject.finalize, loocRedBlackTree_finalize);END_CTOR

/**
 * loocRedBlackTree析构函数
 */
DTOR(loocRedBlackTree)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
