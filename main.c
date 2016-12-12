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

int main(int argc, char **argv) {

	int i = 0;
	/**
	 * 1. 顺序表的操作
	 */
	printf("****************loocSeqList****************\r\n");
	loocSeqList* seqList = loocSeqList_new(looc_file_line);
	seqList->init(seqList, 10, sizeof(int));
	for (i = 0; i < 10; i++) {
		seqList->insert(seqList, &i);
	}
	for (i = 0; i < seqList->length; i++) {
		printf("%d ", *(int*) (seqList->getElement(seqList, i)));
	}
	printf("\r\n");
	seqList->remove(seqList, 3);
	for (i = 0; i < seqList->length; i++) {
		printf("%d ", *(int*) (seqList->getElement(seqList, i)));
	}
	printf("\r\n");
	i = 55;
	seqList->modify(seqList, 5, &i);
	for (i = 0; i < seqList->length; i++) {
		printf("%d ", *(int*) (seqList->getElement(seqList, i)));
	}
	printf("\r\n");
	loocSeqList_delete(seqList);
	/* 报告内存泄漏情况 */
	looc_report();

	/**
	 * 2. 单向链表的操作
	 */
	printf("****************loocSingleList****************\r\n");
	i = 22;
	loocSingleListNode* singleListNode = loocSingleListNode_new(looc_file_line);
	singleListNode->init(singleListNode, sizeof(int), (void*) &i);
	loocSingleList* singleList = loocSingleList_new(looc_file_line);
	singleList->init(singleList, sizeof(int), singleListNode);
	for (i = 0; i < 10; i++) {
		singleList->insertAt(singleList, i, (void*) &i);
	}
	for (i = 0; i < singleList->length; i++) {
		printf("%d ", *(int*) (singleList->getAt(singleList, i)));
	}
	printf("\r\n");
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

