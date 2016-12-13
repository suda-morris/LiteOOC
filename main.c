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
#include <loocBinTree.h>
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
		seqList->insert(seqList, &i);
	}
	/* 打印顺序表中的元素 */
	for (i = 0; i < seqList->length; i++) {
		printf("%d ", *(int*) (seqList->getElement(seqList, i)));
	}
	printf("\r\n");
	/* 删除顺序表中第9个元素 */
	seqList->remove(seqList, 9);
	/* 删除顺序表中第0个元素 */
	seqList->remove(seqList, 0);
	/* 打印顺序表中的元素 */
	for (i = 0; i < seqList->length; i++) {
		printf("%d ", *(int*) (seqList->getElement(seqList, i)));
	}
	printf("\r\n");
	/* 将顺序表中第5个元素修改为55 */
	i = 55;
	seqList->modify(seqList, 5, &i);
	/* 打印顺序表中的元素 */
	for (i = 0; i < seqList->length; i++) {
		printf("%d ", *(int*) (seqList->getElement(seqList, i)));
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
	 * 3. 二叉树的操作
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

