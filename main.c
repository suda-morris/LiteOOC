/*
 ============================================================================
 Name        : main.c
 Author      : morris
 Version     : 1.0
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

#include <loocSeqList.h>
#include <loocSingleList.h>
#include <loocDoubleList.h>
#include <loocCircularList.h>
#include <loocStack.h>
#include <loocQueue.h>
#include <loocHashMap.h>
#include <loocBinTree.h>
#include <loocBinSearchTree.h>
#include <loocRedBlackTree.h>
#include <stdio.h>
#include <lea.h>

#define SEQLIST_LENGTH				(10)
#define SINGLELIST_LENGTH			(5)

/**
 * 针对二叉树节点的操作
 * @param node 当前二叉树节点
 * @param args 参数
 */
static void actionPrint_BinTree(loocBinTreeNode* node, void* args) {
	printf("%c ", *(char*) (node->_data));
}

/**
 * 针对二叉查找树节点的操作
 * @param node 当前二叉查找树节点
 * @param args 参数
 */
static void actionPrint_BinSearchTree(loocBinSearchTreeNode* node, void* args) {
	printf("%d ", *(int*) (node->_data));
}

/**
 * 针对红黑树节点的操作
 * @param node 当前二叉查找树节点
 * @param args 参数
 */
static void actionPrint_RedBlackTree(loocRedBlackTreeNode* node, void* args) {
	printf("%d(%s)", *(int*) (node->_data),
			(node->color == looc_RBT_Black) ? "black" : "red");
	if (node->lChild->_data) {
		printf("\tleft:%d(%s)", *(int*) (node->lChild->_data),
				(node->lChild->color == looc_RBT_Black) ? "black" : "red");
	}
	if (node->rChild->_data) {
		printf("\tright:%d(%s)", *(int*) (node->rChild->_data),
				(node->rChild->color == looc_RBT_Black) ? "black" : "red");
	}
	if (node->parent->_data) {
		printf("\tparent:%d(%s)\r\n", *(int*) (node->parent->_data),
				(node->parent->color == looc_RBT_Black) ? "black" : "red");
	} else {
		printf("\r\n");
	}
}

/**
 * 散列函数(采用除数留余法计算散列地址)
 * @param  cthis 当前Hash表对象指针
 * @param  value 数据指针
 * @return       返回数据的散列地址
 */
static int hash(loocHashMap* cthis, void* value) {
	int data = *(int*) value;
	return data % (cthis->_maxSize);
}

/**
 * 二叉查找树的比较策略
 * @param  cthis 旧节点
 * @param  node  新节点
 * @return       新节点的关键字大于旧节点的关键字就返回1，小于返回-1，等于返回0
 */
static int BST_compareStrategy(loocBinSearchTreeNode* cthis,
		loocBinSearchTreeNode* node) {
	int a = *(int*) cthis->_data;
	int b = *(int*) node->_data;
	if (a > b) {
		return -1;
	} else if (a < b) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * 红黑树的比较策略
 * @param  cthis 旧节点
 * @param  node  新节点
 * @return       新节点的关键字大于旧节点的关键字就返回1，小于返回-1，等于返回0
 */
static int RBT_compareStrategy(loocRedBlackTreeNode* cthis,
		loocRedBlackTreeNode* node) {
	int a = *(int*) cthis->_data;
	int b = *(int*) node->_data;
	if (a > b) {
		return -1;
	} else if (a < b) {
		return 1;
	} else {
		return 0;
	}
}

int main(int argc, char **argv) {

	int i = 0;
	/**
	 * 1. 顺序表的操作
	 */
	printf("****************loocSeqList****************\r\n");
	/* 新建顺序表对象 */
	loocSeqList* seqList = loocSeqList_new(looc_file_line);
	/* 初始化顺序表，并分配好内存空间，顺序表大小为10,元素类型为int */
	seqList->init(seqList, SEQLIST_LENGTH, sizeof(int));
	/* 向顺序表中插入10个数据 */
	for (i = 0; i < SEQLIST_LENGTH; i++) {
		seqList->insert(seqList, (void*) &i);
	}
	/* 打印顺序表中的元素 */
	for (i = 0; i < seqList->length; i++) {
		printf("%d ", *(int*) (seqList->getAt(seqList, i)));
	}
	printf("\r\n");
	/* 删除顺序表中第9个元素 */
	seqList->removeAt(seqList, 9);
	/* 删除顺序表中第0个元素 */
	seqList->removeAt(seqList, 0);
	/* 打印顺序表中的元素 */
	for (i = 0; i < seqList->length; i++) {
		printf("%d ", *(int*) (seqList->getAt(seqList, i)));
	}
	printf("\r\n");
	/* 将顺序表中第5个元素修改66 */
	i = 66;
	seqList->modifyAt(seqList, 5, &i);
	/* 打印顺序表中的元素 */
	for (i = 0; i < seqList->length; i++) {
		printf("%d ", *(int*) (seqList->getAt(seqList, i)));
	}
	printf("\r\n");
	/* 释放顺序表内存空间 */
	loocSeqList_delete(seqList);
	/* 报告内存泄漏情况 */
	looc_report();

	/**
	 * 2. 单向链表的操作
	 */
	printf("****************loocSingleList****************\r\n");
	i = 11;
	/* 创建单向链表对象 */
	loocSingleList* singleList = loocSingleList_new(looc_file_line);
	/* 创建单链表节点对象 */
	loocSingleListNode* singleListNode = loocSingleListNode_new(looc_file_line);
	/* 节点对象初始化 */
	singleListNode->init(singleListNode, sizeof(int), (void*) &i);
	/* 初始化单向链表，创建头结点，分配内存空间,结点元素为int类型 */
	singleList->init(singleList, sizeof(int), singleListNode);
	/* 插入整形数据,插入链表头 */
	i = 22;
	singleList->insertAt(singleList, 0, (void*) &i);
	i = 33;
	singleList->insertAt(singleList, 1, (void*) &i);
	i = 44;
	singleList->insertAt(singleList, 2, (void*) &i);
	i = 55;
	singleList->insertAt(singleList, 3, (void*) &i);
	/* 删除指定位置节点 */
	singleList->removeAt(singleList, 4);
	/* 打印单向链表中的元素 */
	for (i = 0; i < singleList->length; i++) {
		printf("%d ", *(int*) (singleList->getAt(singleList, i)));
	}
	printf("\r\n");
	printf("circle appears at %p\r\n", singleList->haveCircle(singleList));
	/* 释放单向链表内存空间 */
	loocSingleList_delete(singleList);
	/* 报告内存泄漏情况 */
	looc_report();

	/**
	 * 3. 双向链表的操作
	 */
	printf("****************loocDoubleList****************\r\n");
	i = 'a';
	/* 创建双向链表对象 */
	loocDoubleList* doubleList = loocDoubleList_new(looc_file_line);
	/* 创建双向链表节点对象 */
	loocDoubleListNode* doubleListNode = loocDoubleListNode_new(looc_file_line);
	/* 节点对象初始化 */
	doubleListNode->init(doubleListNode, sizeof(int), (void*) &i);
	/* 初始化双向链表 */
	doubleList->init(doubleList, sizeof(int), doubleListNode);
	/* 插入数据 */
	i = 'b';
	doubleList->insertAt(doubleList, 0, (void*) &i);
	i = 'c';
	doubleList->insertAt(doubleList, 1, (void*) &i);
	i = 'd';
	doubleList->insertAt(doubleList, 2, (void*) &i);
	i = 'e';
	doubleList->insertAt(doubleList, 3, (void*) &i);
	/* 删除指定位置节点 */
	doubleList->removeAt(doubleList, 0);
	doubleList->removeAt(doubleList, 3);
	/* 打印双向链表中的数据 */
	for (i = 0; i < doubleList->length; i++) {
		printf("%c ", *(char*) (doubleList->getAt(doubleList, i)));
	}
	printf("\r\n");
	/* 释放双向链表内存 */
	loocDoubleList_delete(doubleList);
	/* 报告内存泄漏情况 */
	looc_report();

	/**
	 * 4. 循环链表的操作
	 */
	printf("****************loocCircularList****************\r\n");
	i = 17;
	/* 创建循环链表对象 */
	loocCircularList* circularList = loocCircularList_new(looc_file_line);
	/* 创建循环链表节点对象 */
	loocCircularListNode* circularListNode = loocCircularListNode_new(
	looc_file_line);
	/* 节点对象初始化 */
	circularListNode->init(circularListNode, sizeof(int), (void*) &i);
	/* 初始化循环链表 */
	circularList->init(circularList, sizeof(int), circularListNode);
	/* 插入数据 */
	i = 18;
	circularList->insertAt(circularList, 0, (void*) &i);
	i = 19;
	circularList->insertAt(circularList, 1, (void*) &i);
	i = 20;
	circularList->insertAt(circularList, 2, (void*) &i);
	i = 21;
	circularList->insertAt(circularList, 3, (void*) &i);
	/* 删除指定位置节点 */
	circularList->removeAt(circularList, 0);
	circularList->removeAt(circularList, 3);
	/* 打印循环链表中的数据 */
	for (i = 0; i < circularList->length; i++) {
		printf("%d ", *(char*) (circularList->getAt(circularList, i)));
	}
	printf("\r\n");
	/* 释放循环链表内存 */
	loocCircularList_delete(circularList);
	/* 报告内存泄漏情况 */
	looc_report();

	/**
	 * 5. 栈的操作
	 */
	printf("****************loocStack****************\r\n");
	/* 创建栈对象 */
	loocStack* stack = loocStack_new(looc_file_line);
	/* 初始化栈 */
	stack->init(stack, 10, sizeof(int));
	/* 压栈操作 */
	for (i = 0; i < 10; i++) {
		stack->push(stack, (void*) &i);
	}
	/* 出栈操作 */
	for (i = 0; i < 10; i++) {
		printf("%d ", *(int*) stack->pop(stack));
	}
	printf("\r\n");
	/* 释放栈内存空间 */
	loocStack_delete(stack);
	/* 报告内存泄漏情况 */
	looc_report();

	/**
	 * 6. 队列的操作
	 */
	printf("****************loocQueue****************\r\n");
	/* 创建队列对象 */
	loocQueue* queue = loocQueue_new(looc_file_line);
	/* 初始化对列 */
	queue->init(queue, 10, sizeof(int));
	/* 入队操作 */
	for (i = 0; i < 15; i++) {
		queue->enqueue(queue, (void*) &i);
	}
	/* 出队操作 */
	for (i = 0; i < 10; i++) {
		printf("%d ", *(int*) queue->dequeue(queue));
	}
	printf("\r\n");
	/* 释放队列内存空间 */
	loocStack_delete(stack);
	/* 报告内存泄漏情况 */
	looc_report();

	/**
	 * 7. 哈希表的操作
	 */
	printf("****************loocHashMap****************\r\n");
	/* 创建Hash表对象 */
	loocHashMap* hashMap = loocHashMap_new(looc_file_line);
	/* 初始化Hash表对象 */
	hashMap->init(hashMap, 13, sizeof(int), hash);
	/* 依次插入元素 */
	i = 66;
	hashMap->insert(hashMap, (void*) &i);
	i = 32;
	hashMap->insert(hashMap, (void*) &i);
	i = 0;
	hashMap->insert(hashMap, (void*) &i);
	i = 478;
	hashMap->insert(hashMap, (void*) &i);
	i = 11;
	hashMap->insert(hashMap, (void*) &i);
	i = 23;
	hashMap->insert(hashMap, (void*) &i);
	i = 43;
	hashMap->insert(hashMap, (void*) &i);
	i = 55;
	hashMap->insert(hashMap, (void*) &i);
	i = 67;
	hashMap->insert(hashMap, (void*) &i);
	i = 108;
	hashMap->insert(hashMap, (void*) &i);
	i = 230;
	hashMap->insert(hashMap, (void*) &i);
	i = 223;
	hashMap->insert(hashMap, (void*) &i);
	i = 10;
	hashMap->insert(hashMap, (void*) &i);
	/* 输出Hash表中元素 */
	for (i = 0; i < 13; i++) {
		printf("{%d:%d} ", i, *(int*) hashMap->getAt(hashMap, i));
	}
	printf("\r\n");
	/* 打印Hash表有效长度 */
	printf("Total %d elements\r\n", hashMap->length);
	/* 查找元素55 */
	i = 55;
	printf("Position of 55: %d\r\n", hashMap->search(hashMap, (void*) &i));
	/* 释放Hash表内存空间 */
	loocHashMap_delete(hashMap);
	/* 报告内存泄漏情况 */
	looc_report();

	/**
	 * 8. 二叉树的操作
	 */
	printf("****************loocBinTree****************\r\n");
	i = 'A';
	/* 创建二叉树对象 */
	loocBinTree* binTree = loocBinTree_new(looc_file_line);
	/* 创建二叉树节点对象 */
	loocBinTreeNode* binTreeNode = loocBinTreeNode_new(looc_file_line);
	/* 节点对象初始化 */
	binTreeNode->init(binTreeNode, sizeof(int), (void*) &i);
	/* 二叉树对象初始化，指定根节点 */
	binTree->init(binTree, sizeof(int), binTreeNode);
	/* 增子节点 */
	i = 'B';
	binTreeNode->setLeftChild(binTreeNode, (void*) &i);
	i = 'C';
	binTreeNode->setRightChild(binTreeNode, (void*) &i);
	i = 'D';
	binTreeNode->lChild->setLeftChild(binTreeNode->lChild, (void*) &i);
	i = 'E';
	binTreeNode->lChild->setRightChild(binTreeNode->lChild, (void*) &i);
	i = 'F';
	binTreeNode->rChild->setLeftChild(binTreeNode->rChild, (void*) &i);
	i = 'G';
	binTreeNode->rChild->setRightChild(binTreeNode->rChild, (void*) &i);
	i = 'H';
	binTreeNode->lChild->lChild->setLeftChild(binTreeNode->lChild->lChild,
			(void*) &i);
	i = 'I';
	binTreeNode->lChild->lChild->setRightChild(binTreeNode->lChild->lChild,
			(void*) &i);
	i = 'J';
	binTreeNode->lChild->rChild->setRightChild(binTreeNode->lChild->rChild,
			(void*) &i);
	/* 前序遍历打印节点 */
	binTree->preOrder(binTree, actionPrint_BinTree, NULL);
	printf("\r\n");
	/* 中序遍历打印节点 */
	binTree->inOrder(binTree, actionPrint_BinTree, NULL);
	printf("\r\n");
	/* 后序遍历打印节点 */
	binTree->postOrder(binTree, actionPrint_BinTree, NULL);
	printf("\r\n");
	/* 层序遍历打印节点 */
	binTree->layerOrder(binTree, actionPrint_BinTree, NULL);
	printf("\r\n");
	/* 打印已知节点的父节点 */
	printf("node E's Parent is :%c\r\n",
			*(char*) (binTree->getParent(binTree, binTree->root->lChild->rChild)->_data));
	/* 打印已知节点的兄弟节点 */
	printf("node E's Brother is :%c\r\n",
			*(char*) (binTree->getBrother(binTree,
					binTree->root->lChild->rChild)->_data));
	/* 打印树的高度 */
	printf("Height of Tree: %d\r\n", binTree->getHeight(binTree));
	/* 打印指定节点的深度 */
	printf("Depth of node D: %d\r\n",
			binTree->getDepthOfNode(binTree, binTree->root->lChild->lChild));
	/* 释放二叉树对象内存空间 */
	loocBinTree_delete(binTree);
	/* 报告内存泄漏情况 */
	looc_report();

	/**
	 * 9. 二叉查找树的操作
	 */
	printf("****************loocBinSearchTree****************\r\n");
	i = 100;
	/* 创建二叉查找树节点对象 */
	loocBinSearchTreeNode* binSearchTreeNode = loocBinSearchTreeNode_new(
	looc_file_line);
	/* 初始化二叉查找树节点 */
	binSearchTreeNode->init(binSearchTreeNode, sizeof(int), (void*) &i);
	/* 创建二叉查找树对象 */
	loocBinSearchTree* binSearchTree = loocBinSearchTree_new(looc_file_line);
	/* 初始化二叉查找树,并设置根节点 */
	binSearchTree->init(binSearchTree, sizeof(int), binSearchTreeNode,
			BST_compareStrategy);
	/* 往二叉查找树中插入数据 */
	i = 77;
	binSearchTree->insert(binSearchTree, (void*) &i);
	i = 103;
	binSearchTree->insert(binSearchTree, (void*) &i);
	i = 191;
	binSearchTree->insert(binSearchTree, (void*) &i);
	i = 88;
	binSearchTree->insert(binSearchTree, (void*) &i);
	i = 143;
	binSearchTree->insert(binSearchTree, (void*) &i);
	i = 43;
	binSearchTree->insert(binSearchTree, (void*) &i);
	i = 51;
	binSearchTree->insert(binSearchTree, (void*) &i);
	i = 152;
	binSearchTree->insert(binSearchTree, (void*) &i);
	i = 6;
	binSearchTree->insert(binSearchTree, (void*) &i);
	i = 200;
	binSearchTree->insert(binSearchTree, (void*) &i);
	i = 199;
	binSearchTree->insert(binSearchTree, (void*) &i);
	/* 中序遍历打印节点,即排序 */
	binSearchTree->inOrder(binSearchTree, actionPrint_BinSearchTree, NULL);
	printf("\r\n");
	/* 查询操作 */
	i = 100;
	binSearchTreeNode = binSearchTree->search(binSearchTree, (void*) &i);
	if (binSearchTreeNode) {
		if (binSearchTreeNode->parent) {
			printf("Parent of node %d is node %d\r\n", i,
					*(int*) (binSearchTreeNode->parent->_data));
		}
		/* 删除指定节点 */
		binSearchTree->deleteNode(binSearchTree, binSearchTreeNode);
	}
	/* 中序遍历打印节点,即排序 */
	binSearchTree->inOrder(binSearchTree, actionPrint_BinSearchTree, NULL);
	printf("\r\n");
	/* 打印最大最小值 */
	printf("Max:%d\tMin:%d\r\n",
			*(int*) binSearchTree->getMaxNode(binSearchTree)->_data,
			*(int*) binSearchTree->getMinNode(binSearchTree)->_data);
	/* 释放二叉查找树对象内存空间 */
	loocBinSearchTree_delete(binSearchTree);
	/* 报告内存泄漏情况 */
	looc_report();

	/**
	 * 10. 红黑树的操作
	 */
	printf("****************loocRedBlackTree****************\r\n");
	i = 1;
	/* 创建红黑树节点对象 */
	loocRedBlackTreeNode* redBlackTreeNode = loocRedBlackTreeNode_new(
	looc_file_line);
	/* 初始化红黑树节点 */
	redBlackTreeNode->init(redBlackTreeNode, sizeof(int), (void*) &i,
			looc_RBT_Black);
	/* 创建红黑树对象 */
	loocRedBlackTree* redBlackTree = loocRedBlackTree_new(looc_file_line);
	/* 初始化红黑树对象 */
	redBlackTree->init(redBlackTree, sizeof(int), redBlackTreeNode,
			RBT_compareStrategy);
	/* 红黑树插入结点操作 */
	for (i = 2; i < 15; i++) {
		redBlackTree->insert(redBlackTree, (void*) &i);
	}
	/* 中序遍历打印节点,即排序 */
	redBlackTree->inOrder(redBlackTree, actionPrint_RedBlackTree, NULL);
	/* 红黑树的查找操作 */
	i = 10;
	redBlackTreeNode = redBlackTree->search(redBlackTree, (void*) &i);
	/* 红黑树删除节点操作 */
	redBlackTree->deleteNode(redBlackTree, redBlackTreeNode);
	/* 中序遍历打印节点,即排序 */
	redBlackTree->inOrder(redBlackTree, actionPrint_RedBlackTree, NULL);
	/* 释放红黑树对象内存空间 */
	loocRedBlackTree_delete(redBlackTree);
	/* 报告内存泄漏情况 */
	looc_report();

	/* 算法测试 */
	/* 判断数组是否是某个二叉查找树的后序遍历 */
	printf("****************verifySquenceOfBST****************\r\n");
	int test_squence[] = { 5, 7, 6, 9, 11, 10, 8 };
	printf("verifySquenceOfBST %s\r\n",
			verifySquenceOfBST(test_squence, 7) ? "pass" : "not pass");

	return 0;
}

