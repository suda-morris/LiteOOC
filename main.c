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
#include <stdio.h>

int main(int argc, char **argv) {

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

