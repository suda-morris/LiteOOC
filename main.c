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
#include <loocBinTree.h>
#include <loocCircularList.h>
#include <loocStack.h>
#include <loocQueue.h>
#include <stdio.h>

#define SEQLIST_LENGTH				(10)
#define SINGLELIST_LENGTH			(5)

static void actionPrint(loocBinTreeNode* node, void* args) {
	printf("%d ", *(int*) (node->_data));
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
	 * 7. 二叉树的操作
	 */
	printf("****************loocBinTree****************\r\n");
	i = 10;
	/* 创建二叉树对象 */
	loocBinTree* binTree = loocBinTree_new(looc_file_line);
	/* 创建二叉树节点对象 */
	loocBinTreeNode* binTreeNode = loocBinTreeNode_new(looc_file_line);
	/* 节点对象初始化 */
	binTreeNode->init(binTreeNode, sizeof(int), (void*) &i);
	/* 二叉树对象初始化，指定根节点 */
	binTree->init(binTree, sizeof(int), binTreeNode);
	/* 增加左子节点 */
	i = 20;
	binTreeNode->setLeftChild(binTreeNode, (void*) &i);
	/* 增加右子节点 */
	i = 30;
	binTreeNode->setRightChild(binTreeNode, (void*) &i);
	i = 40;
	binTreeNode->lChild->setLeftChild(binTreeNode->lChild, (void*) &i);
	i = 50;
	binTreeNode->rChild->setRightChild(binTreeNode->rChild, (void*) &i);
	/* 前序遍历打印节点 */
	binTree->preOrder(binTree, actionPrint, NULL);
	printf("\r\n");
	/* 中序遍历打印节点 */
	binTree->inOrder(binTree, actionPrint, NULL);
	printf("\r\n");
	/* 后序遍历打印节点 */
	binTree->postOrder(binTree, actionPrint, NULL);
	printf("\r\n");
	/* 释放二叉树对象内存空间 */
	loocBinTree_delete(binTree);
	/* 报告内存泄漏情况 */
	looc_report();

	return 0;
}

