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
#include <stdio.h>

/**
 * 增加左子树
 * @param cthis 当前树
 * @param sleft 欲添加的左子树
 */
static void loocBinTree_addSubLeft(loocBinTree* cthis, loocBinTree* sleft) {
	cthis->left = sleft;
	/* 增加左子树的引用计数 */
	sleft->loocObject._use++;
}

/**
 * 增加右子树
 * @param cthis  当前树
 * @param sright 欲添加的右子树
 */
static void loocBinTree_addSubRight(loocBinTree* cthis, loocBinTree* sright) {
	cthis->right = sright;
	/* 增加右子树的引用计数 */
	sright->loocObject._use++;
}

/**
 * loocBinTree的构造函数
 */
ABS_CTOR(loocBinTree)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->left = NULL;
	cthis->right = NULL;
	/* 成员函数的绑定 */
	FUNCTION_SETTING(addSubLeft, loocBinTree_addSubLeft);
	FUNCTION_SETTING(addSubRight, loocBinTree_addSubRight);END_ABS_CTOR

/**
 * loocBinTree的析构函数
 */
DTOR(loocBinTree)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	loocObject_dtor(SUPER_PTR(cthis, loocObject));
END_DTOR

/**
 * 整型树的成员变量初始化
 * @param cthis 当前整型树
 * @param value 成员变量初始化值
 */
static void loocBinTree_Int_init(loocBinTree_Int* cthis, int value) {
	cthis->nodeValue = value;
}

/**
 * 实现loocBinTree类的print接口,前序遍历，递归方式
 * @param cthis loocBinTree对象指针
 */
static void loocBinTree_Int_print(loocBinTree* cthis) {
	loocBinTree_Int* tree = SUB_PTR(cthis, loocBinTree, loocBinTree_Int);
	printf("%d ", tree->nodeValue);
	if (cthis->left) {
		cthis->left->print(cthis->left);
	}
	if (cthis->right) {
		cthis->right->print(cthis->right);
	}
}

/**
 * 实现loocObject类的finalize接口
 * @param object loocObject对象指针
 */
static void loocBinTree_Int_finalize(loocObject* object) {
	/* 类型转化为具体的对象 */
	loocBinTree* tree = SUB_PTR(object, loocObject, loocBinTree);
	/* 如果成员变量中包含子对象，需要一一删除（注意先判断引用计数）*/
	if (tree->left) {
		if (tree->left->loocObject._use) {
			tree->left->loocObject._use--;
		}
		if (tree->left->loocObject._use == 0) {
			/* 实质上最终还是调用了当前函数 */
			loocBinTree_delete(tree->left);
		}
	}
	if (tree->right) {
		if (tree->right->loocObject._use) {
			tree->right->loocObject._use--;
		}
		if (tree->right->loocObject._use == 0) {
			loocBinTree_delete(tree->right);
		}
	}
}

/**
 * loocBinTree_Int的构造函数
 */
CTOR(loocBinTree_Int)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocBinTree);
	/* 成员函数的绑定 */
	FUNCTION_SETTING(init, loocBinTree_Int_init);
	FUNCTION_SETTING(loocBinTree.print, loocBinTree_Int_print);
	FUNCTION_SETTING(loocBinTree.loocObject.finalize, loocBinTree_Int_finalize);END_CTOR

/**
 * loocBinTree_Int的析构函数
 */
DTOR(loocBinTree_Int)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	loocBinTree_dtor(SUPER_PTR(cthis, loocBinTree));
END_DTOR
