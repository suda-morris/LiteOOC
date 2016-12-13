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
	/* 创建单向链表对象 */
	loocSingleList* singleList = loocSingleList_new(looc_file_line);
	/* 初始化单向链表，创建头结点，分配内存空间,结点元素为int类型 */
	singleList->init(singleList, sizeof(int), NULL);
	/* 插入整形数据,插入链表头 */
	for (i = 0; i < SINGLELIST_LENGTH; i++) {
		singleList->insertAt(singleList, 0, (void*) &i);
	}
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
	loocBinTree_Int* root = loocBinTree_Int_new(looc_file_line);
	root->init(root, 10);

	loocBinTree_Int* lsub = loocBinTree_Int_new(looc_file_line);
	lsub->init(lsub, 20);
	root->loocBinTree.addSubLeft(SUPER_PTR(root, loocBinTree),
			SUPER_PTR(lsub, loocBinTree));

	loocBinTree_Int* rsub = loocBinTree_Int_new(looc_file_line);
	rsub->init(rsub, 30);
	root->loocBinTree.addSubRight(SUPER_PTR(root, loocBinTree),
			SUPER_PTR(rsub, loocBinTree));

	loocBinTree_Int* node = loocBinTree_Int_new(looc_file_line);
	node->init(node, 40);
	lsub->loocBinTree.addSubRight(SUPER_PTR(lsub, loocBinTree),
			SUPER_PTR(node, loocBinTree));
	rsub->loocBinTree.addSubLeft(SUPER_PTR(rsub, loocBinTree),
			SUPER_PTR(node, loocBinTree));

	node = loocBinTree_Int_new(looc_file_line);
	node->init(node, 50);
	rsub->loocBinTree.addSubRight(SUPER_PTR(rsub, loocBinTree),
			SUPER_PTR(node, loocBinTree));

	root->loocBinTree.print(SUPER_PTR(root, loocBinTree));
	printf("\r\n");

	loocBinTree_Int_delete(root);

	/* 报告内存泄漏情况 */
	looc_report();

	return 0;
}

