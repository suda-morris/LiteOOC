/*
 ============================================================================
 Name        : main.c
 Author      : morris
 Version     : 1.0
 Copyright   : Your copyright notice
 Description :
 ============================================================================
 */

#include <loocBinTree.h>
#include <loocSeqList.h>
#include <stdio.h>

int main(int argc, char **argv) {

	int i = 0;
	/**
	 * 1. 顺序表的操作
	 */
	loocSeqList* list = loocSeqList_new(looc_file_line);
	list->init(list, 10, sizeof(int));
	for (i = 0; i < 10; i++) {
		list->insert(list, &i);
	}
	for (i = 0; i < list->length; i++) {
		printf("%d ", *(int*) (list->getElement(list, i)));
	}
	printf("\r\n");
	list->remove(list, 3);
	for (i = 0; i < list->length; i++) {
		printf("%d ", *(int*) (list->getElement(list, i)));
	}
	printf("\r\n");
	i = 55;
	list->modify(list, 5, &i);
	for (i = 0; i < list->length; i++) {
		printf("%d ", *(int*) (list->getElement(list, i)));
	}
	printf("\r\n");
	loocSeqList_delete(list);
	/* 报告内存泄漏情况 */
	looc_report();

	/**
	 * 2. 二叉树的操作
	 */
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

