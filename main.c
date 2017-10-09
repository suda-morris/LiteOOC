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
#include <loocHeap.h>
#include <loocAdjacencyGraph.h>
#include <loocLinkedGraph.h>
#include <loocDisjointSet.h>
#include <loocMatrix.h>
#include <loocString.h>
#include <lea.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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
 * 针对图的节点的操作
 * @param node 当前图顶点节点
 * @param args 参数
 */
static void actionPrint_Graph(void* node, void* args) {
	printf("%c ", *(char*) node);
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
 * 堆的比较策略
 * @param  old 旧节点
 * @param  new 新节点
 * @return     新节点的关键字大于旧节点的关键字就返回1，小于返回-1，等于返回0
 */
static int Heap_compareStrategy(void* old, void* new) {
	int a = *(int*) old;
	int b = *(int*) new;
	if (a > b) {
		return -1;
	} else if (a < b) {
		return 1;
	} else {
		return 0;
	}
}

static void FloydDisplaySinglePath(
		int path[][LOOC_DEFAULT_LOOCADJACENCYGRAPH_VERTEX], int i, int j) {
	int k = path[i][j];
	if (k != -1) {
		FloydDisplaySinglePath(path, i, k);
		printf("%d ", k);
		FloydDisplaySinglePath(path, k, j);
	}
}

/**
 * 打印多源最短路径（每对顶点的最短路径）
 * @param  cthis 当前图对象指针
 * @param  D     保存每对顶点的最短路径的权值
 * @param  path  记录最短路径上中间点的前驱点
 */
static void FloydDisplayPaths(loocAdjacencyGraph* cthis,
		int D[][LOOC_DEFAULT_LOOCADJACENCYGRAPH_VERTEX],
		int path[][LOOC_DEFAULT_LOOCADJACENCYGRAPH_VERTEX]) {
	int i, j;
	for (i = 0; i < cthis->numV; i++) {
		for (j = 0; j < cthis->numV; j++) {
			if (D[i][j] != LOOC_GRAPH_NO_EDGE) {
				printf("顶点%d到顶点%d的最短路径权值为%d\r\n", i, j, D[i][j]);
				printf("%d ", i);
				FloydDisplaySinglePath(path, i, j);
				printf("%d\r\n", j);
			}
		}
	}
}

/**
 * 欲求定积分的表达式
 */
static double expression(double x) {
	if (x != 0) {
		return sin(x) / x;
	} else {
		return 1;
	}
}

int main(int argc, char **argv) {

	int i = 0, j = 0;
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
	loocQueue_delete(queue);
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
	 * 11. 堆的操作
	 */
	printf("****************loocHeap****************\r\n");
	/* 创建堆对象 */
	loocHeap* heap = loocHeap_new(looc_file_line);
	/* 初始化堆对象(最小堆),存储int值 */
	heap->init(heap, LOOC_MIN_HEAP, 10, sizeof(int), Heap_compareStrategy);
	/* 插入数据 */
	i = 77;
	heap->insert(heap, (void*) &i);
	i = 101;
	heap->insert(heap, (void*) &i);
	i = 98;
	heap->insert(heap, (void*) &i);
	i = 43;
	heap->insert(heap, (void*) &i);
	i = 110;
	heap->insert(heap, (void*) &i);
	i = 140;
	heap->insert(heap, (void*) &i);
	for (i = 0; i < 6; i++) {
		/* 打印堆中的根节点元素 */
		printf("%d\r\n", *(int*) (heap->getRoot(heap)));
		/* 删除堆中的根节点元素 */
		heap->deleteRoot(heap);
	}
	/* 释放堆对象内存空间 */
	loocHeap_delete(heap);
	/* 报告内存泄露情况 */
	looc_report();

	/**
	 * 12. 图（邻接矩阵存储）的操作
	 */
	printf("****************loocAdjacencyGraph****************\r\n");
	/* 创建图对象 */
	loocAdjacencyGraph* adjGraph = loocAdjacencyGraph_new(looc_file_line);
	/* 初始化有向图，顶点元素为int数，最多10个顶点 */
	adjGraph->init(adjGraph, 10, sizeof(int), 1);
	/* 增加顶点 */
	i = 'A';
	adjGraph->addVertex(adjGraph, (void*) &i);
	i = 'B';
	adjGraph->addVertex(adjGraph, (void*) &i);
	i = 'C';
	adjGraph->addVertex(adjGraph, (void*) &i);
	i = 'D';
	adjGraph->addVertex(adjGraph, (void*) &i);
	i = 'E';
	adjGraph->addVertex(adjGraph, (void*) &i);
	i = 'F';
	adjGraph->addVertex(adjGraph, (void*) &i);
	/* 增加边 */
	adjGraph->insertEdge(adjGraph, 0, 1, 6);
	adjGraph->insertEdge(adjGraph, 0, 2, 3);
	adjGraph->insertEdge(adjGraph, 2, 1, 2);
	adjGraph->insertEdge(adjGraph, 1, 3, 5);
	adjGraph->insertEdge(adjGraph, 2, 3, 3);
	adjGraph->insertEdge(adjGraph, 3, 5, 3);
	adjGraph->insertEdge(adjGraph, 3, 4, 2);
	adjGraph->insertEdge(adjGraph, 2, 4, 4);
	adjGraph->insertEdge(adjGraph, 4, 5, 5);
	/* 拓扑排序 */
	int topOrder_adj[10];
	adjGraph->topologySort(adjGraph, topOrder_adj);
	for (i = 0; i < adjGraph->numV; i++) {
		printf("%d\t", topOrder_adj[i]);
	}
	printf("\r\n");
	/* 输出v0到各顶点的最短路径 */
	int dist[10];
	int path[10];
	int s = 0;
	adjGraph->Dijkstra(adjGraph, s, dist, path); //Dijkstra算法
	for (i = 0; i < adjGraph->numV; i++) {
		int stack[10];
		int top = 0;
		stack[top++] = i; //终点
		int tempV = path[i]; //终点的前驱点
		/* 循环知道没有前驱点或者前驱点是源点s */
		while (tempV >= 0 && tempV != s) {
			stack[top++] = tempV;
			tempV = path[tempV];
		}
		stack[top] = s; //起点
		/* 从起点开始输出路径 */
		for (j = top; j >= 0; j--) {
			if (j) {
				printf("%d-->", stack[j]);
			} else {
				printf("%d:路径长度%d\r\n", stack[j], dist[i]);
			}
		}
	}
	/* 多源最短路径 */
	int D[10][10];
	int pathij[10][10];
	adjGraph->Floyd(adjGraph, D, pathij);
	FloydDisplayPaths(adjGraph, D, pathij);
	/* 计算v3的出度 */
	printf("out degree of node:%d\r\n", adjGraph->outDegree(adjGraph, 3));
	/* 删除边 <v0,v1>*/
	adjGraph->deleteEdge(adjGraph, 0, 1);
	/* 计算v5的入度 */
	printf("in degree of node:%d\r\n", adjGraph->inDegree(adjGraph, 5));
	/* 深度优先遍历,从0号顶点开始 */
	adjGraph->DFS(adjGraph, 0, actionPrint_Graph, NULL);
	printf("\r\n");
	/* 广度优先遍历，从0号节点开始 */
	adjGraph->BFS(adjGraph, 0, actionPrint_Graph, NULL);
	printf("\r\n");
	/* 释放图对象内存空间 */
	loocAdjacencyGraph_delete(adjGraph);
	/* 报告内存泄露情况 */
	looc_report();

	/**
	 * 13. 图（邻接表存储）的操作
	 */
	printf("****************loocLinkedGraph****************\r\n");
	/* 创建图对象 */
	loocLinkedGraph* linkGraph = loocLinkedGraph_new(looc_file_line);
	/* 初始化有向图，顶点元素为int数，最多10个顶点 */
	linkGraph->init(linkGraph, 10, sizeof(int), 1);
	/* 增加顶点 */
	i = 'A';
	linkGraph->addVertex(linkGraph, (void*) &i);
	i = 'B';
	linkGraph->addVertex(linkGraph, (void*) &i);
	i = 'C';
	linkGraph->addVertex(linkGraph, (void*) &i);
	i = 'D';
	linkGraph->addVertex(linkGraph, (void*) &i);
	i = 'E';
	linkGraph->addVertex(linkGraph, (void*) &i);
	i = 'F';
	linkGraph->addVertex(linkGraph, (void*) &i);
	/* 增加边 */
	linkGraph->insertEdge(linkGraph, 0, 1, 6);
	linkGraph->insertEdge(linkGraph, 0, 2, 3);
	linkGraph->insertEdge(linkGraph, 2, 1, 2);
	linkGraph->insertEdge(linkGraph, 1, 3, 5);
	linkGraph->insertEdge(linkGraph, 2, 3, 3);
	linkGraph->insertEdge(linkGraph, 3, 5, 3);
	linkGraph->insertEdge(linkGraph, 3, 4, 2);
	linkGraph->insertEdge(linkGraph, 2, 4, 4);
	linkGraph->insertEdge(linkGraph, 4, 5, 5);
	/* 拓扑排序 */
	int topOrder_link[10];
	linkGraph->topologySort(linkGraph, topOrder_link);
	for (i = 0; i < linkGraph->numV; i++) {
		printf("%d\t", topOrder_link[i]);
	}
	printf("\r\n");
	/* 输出v0到各顶点的最短路径 */
	s = 0;
	linkGraph->Dijkstra(linkGraph, s, dist, path); //Dijkstra算法
	for (i = 0; i < linkGraph->numV; i++) {
		int stack[10];
		int top = 0;
		stack[top++] = i; //终点
		int tempV = path[i]; //终点的前驱点
		/* 循环知道没有前驱点或者前驱点是源点s */
		while (tempV >= 0 && tempV != s) {
			stack[top++] = tempV;
			tempV = path[tempV];
		}
		stack[top] = s; //起点
		/* 从起点开始输出路径 */
		for (j = top; j >= 0; j--) {
			if (j) {
				printf("%d-->", stack[j]);
			} else {
				printf("%d:路径长度%d\r\n", stack[j], dist[i]);
			}
		}
	}
	/* 多源最短路径 */
	linkGraph->Floyd(linkGraph, D, pathij);
	FloydDisplayPaths(linkGraph, D, pathij);
	/* 计算v3的出度 */
	printf("out degree of node:%d\r\n", linkGraph->outDegree(linkGraph, 3));
	/* 删除边 <v0,v1>*/
	linkGraph->deleteEdge(linkGraph, 0, 1);
	/* 计算v5的入度 */
	printf("in degree of node:%d\r\n", linkGraph->inDegree(linkGraph, 5));
	/* 深度优先遍历,从0号顶点开始 */
	linkGraph->DFS(linkGraph, 0, actionPrint_Graph, NULL);
	printf("\r\n");
	/* 广度优先遍历，从0号节点开始 */
	linkGraph->BFS(linkGraph, 0, actionPrint_Graph, NULL);
	printf("\r\n");
	/* 释放图对象内存空间 */
	loocLinkedGraph_delete(linkGraph);
	/* 报告内存泄露情况 */
	looc_report();

	/**
	 * 14. 并查集的操作
	 */
	printf("****************loocDisjointSet****************\r\n");
	/* 创建并查集对象 */
	loocDisjointSet* set = loocDisjointSet_new(looc_file_line);
	/* 初始化并查集对象,存储int值 ,最多10个数据*/
	set->init(set, 10, sizeof(int));
	/* 插入数据 */
	for (i = 1; i <= 10; i++) {
		set->insert(set, (void*) &i);
	}
	/* 并操作 */
	j = 6;
	i = 9;
	set->Union(set, (void*) &i, (void*) &j);
	i = 10;
	set->Union(set, (void*) &i, (void*) &j);
	j = 3;
	i = 5;
	set->Union(set, (void*) &i, (void*) &j);
	i = 8;
	set->Union(set, (void*) &i, (void*) &j);
	j = 1;
	i = 2;
	set->Union(set, (void*) &i, (void*) &j);
	i = 4;
	set->Union(set, (void*) &i, (void*) &j);
	i = 7;
	set->Union(set, (void*) &i, (void*) &j);
	i = 3;
	set->Union(set, (void*) &i, (void*) &j);
	/* 打印并查集 */
	for (i = 0; i < 10; i++) {
		printf("data:%d,parent:%d\r\n",
				*(int*) (set->data_pool + i * set->_elementSize),
				set->parent[i]);
	}
	/* 释放图对象内存空间 */
	loocDisjointSet_delete(set);
	/* 报告内存泄露情况 */
	looc_report();

	/* 算法测试 */
	/* 判断数组是否是某个二叉查找树的后序遍历 */
	printf("****************verifySquenceOfBST****************\r\n");
	int test_squence[] = { 5, 7, 6, 9, 11, 10, 8 };
	printf("verifySquenceOfBST %s\r\n",
			verifySquenceOfBST(test_squence, 7) ? "pass" : "not pass");

	/* 数制转换 */
	char result[16];
	transNum(76374, 16, result);
	printf("%s\r\n", result);
	transNum(234, 2, result);
	printf("%s\r\n", result);

	/* 大小端模式 */
	printf("Machine %s little endian\r\n", isLittleEndian() ? "is" : "isn't");

	/* 判断完全平方数 */
	i = 144;
	printf("%d %s Perfect Square\r\n", i, isPerfectSquare(i) ? "is" : "isn't");

	/* 计算斐波那契数 */
	i = 5;
	printf("第%d个斐波那契数:%d\r\n", i, Fibonacci(i));

	/* 插入排序 */
	int data1[10] = { 45, 43, 23, 76, 897, 567, 32, 0, 24, 43 };
	insertSort(data1, 10);
	for (i = 0; i < 10; i++) {
		printf("%d\t", data1[i]);
	}
	printf("\r\n");

	/* 冒泡排序 */
	int data2[10] = { 23, 43, 23, 19, 87, 65, 78, 90, 7, 10 };
	bubbleSort(data2, 10);
	for (i = 0; i < 10; i++) {
		printf("%d\t", data2[i]);
	}
	printf("\r\n");

	/* 快速排序 */
	int data3[10] = { 23, 32, 76, 43, 11, 88, 23, 76, 23, 90 };
	quickSort(data3, 0, 9);
	for (i = 0; i < 10; i++) {
		printf("%d\t", data3[i]);
	}
	printf("\r\n");

	/* 归并排序 */
	int data4[10] = { 32, 26, 15, 89, 87, 100, 654, 54, 33, 26 };
	mergeSort(data4, 10);
	for (i = 0; i < 10; i++) {
		printf("%d\t", data4[i]);
	}
	printf("\r\n");

	/* 希尔排序 */
	int data5[10] = { 32, 26, 15, 89, 87, 100, 654, 54, 33, 26 };
	shellSort(data5, 10);
	for (i = 0; i < 10; i++) {
		printf("%d\t", data5[i]);
	}
	printf("\r\n");

	/* 选择排序 */
	int data6[10] = { 32, 26, 15, 89, 87, 100, 654, 54, 33, 26 };
	selectionSort(data6, 10);
	for (i = 0; i < 10; i++) {
		printf("%d\t", data6[i]);
	}
	printf("\r\n");

	/* 堆排序 */
	int data7[10] = { 32, 26, 15, 89, 87, 100, 654, 54, 33, 26 };
	heapSort(data7, 10);
	for (i = 0; i < 10; i++) {
		printf("%d\t", data7[i]);
	}
	printf("\r\n");

	/* 计数排序 */
	int data8[10] = { 30, 54, 87, 98, 86, 34, 25, 55, 41, 67 };
	countingSort(data8, 10);
	for (i = 0; i < 10; i++) {
		printf("%d\t", data8[i]);
	}
	printf("\r\n");

	/* 基数排序 */
	int data9[10] = { 35, 64, 88, 87, 896, 354, 15, 64, 1, 123 };
	radixSort(data9, 10, 3, 10);
	for (i = 0; i < 10; i++) {
		printf("%d\t", data9[i]);
	}
	printf("\r\n");

	/* 二分查找 */
	printf("key:88@ %d\r\n", binSearch(data9, 10, 88));

	/* 最小生成树算法Prim */
	/* 邻接矩阵存储 */
	/* 创建图对象 */
	adjGraph = loocAdjacencyGraph_new(looc_file_line);
	/* 初始化无向图，顶点元素为int类型，最多10个顶点 */
	adjGraph->init(adjGraph, 10, sizeof(int), 0);
	/* 增加顶点 */
	i = 'A';
	adjGraph->addVertex(adjGraph, (void*) &i);
	i = 'B';
	adjGraph->addVertex(adjGraph, (void*) &i);
	i = 'C';
	adjGraph->addVertex(adjGraph, (void*) &i);
	i = 'D';
	adjGraph->addVertex(adjGraph, (void*) &i);
	i = 'E';
	adjGraph->addVertex(adjGraph, (void*) &i);
	i = 'F';
	adjGraph->addVertex(adjGraph, (void*) &i);
	i = 'G';
	adjGraph->addVertex(adjGraph, (void*) &i);
	i = 'H';
	adjGraph->addVertex(adjGraph, (void*) &i);
	i = 'I';
	adjGraph->addVertex(adjGraph, (void*) &i);
	/* 增加边 */
	adjGraph->insertEdge(adjGraph, 0, 1, 4);
	adjGraph->insertEdge(adjGraph, 0, 7, 8);
	adjGraph->insertEdge(adjGraph, 1, 2, 8);
	adjGraph->insertEdge(adjGraph, 1, 7, 11);
	adjGraph->insertEdge(adjGraph, 2, 3, 7);
	adjGraph->insertEdge(adjGraph, 2, 5, 4);
	adjGraph->insertEdge(adjGraph, 2, 8, 2);
	adjGraph->insertEdge(adjGraph, 3, 4, 9);
	adjGraph->insertEdge(adjGraph, 3, 5, 14);
	adjGraph->insertEdge(adjGraph, 4, 5, 10);
	adjGraph->insertEdge(adjGraph, 5, 6, 2);
	adjGraph->insertEdge(adjGraph, 6, 7, 1);
	adjGraph->insertEdge(adjGraph, 6, 8, 6);
	/* 存放最小生成树 */
	loocAdjacencyGraph* MST_adj = loocAdjacencyGraph_new(looc_file_line);
	MST_adj->init(MST_adj, adjGraph->numV, sizeof(int), adjGraph->check);
	/* 调用最小生成树算法Prim或者Kruskal */
	adjGraph->Kruskal(adjGraph, MST_adj);
	/* 打印最小生成树 */
	for (i = 0; i < MST_adj->numV; i++) {
		for (j = 0; j < MST_adj->numV; j++) {
			int edgeValue = MST_adj->getValueOfEdge(MST_adj, i, j);
			if (edgeValue < LOOC_GRAPH_NO_EDGE) {
				printf("%d\t", edgeValue);
			} else {
				printf("-\t");
			}
		}
		printf("\r\n");
	}
	/* 释放图对象内存空间 */
	loocAdjacencyGraph_delete(adjGraph);
	loocAdjacencyGraph_delete(MST_adj);
	/* 报告内存泄露情况 */
	looc_report();

	/* 邻接链表存储 */
	linkGraph = loocLinkedGraph_new(looc_file_line);
	/* 初始化无向图，顶点元素为int数，最多10个顶点 */
	linkGraph->init(linkGraph, 10, sizeof(int), 0);
	/* 增加顶点 */
	i = 'A';
	linkGraph->addVertex(linkGraph, (void*) &i);
	i = 'B';
	linkGraph->addVertex(linkGraph, (void*) &i);
	i = 'C';
	linkGraph->addVertex(linkGraph, (void*) &i);
	i = 'D';
	linkGraph->addVertex(linkGraph, (void*) &i);
	i = 'E';
	linkGraph->addVertex(linkGraph, (void*) &i);
	i = 'F';
	linkGraph->addVertex(linkGraph, (void*) &i);
	i = 'G';
	linkGraph->addVertex(linkGraph, (void*) &i);
	i = 'H';
	linkGraph->addVertex(linkGraph, (void*) &i);
	i = 'I';
	linkGraph->addVertex(linkGraph, (void*) &i);
	/* 增加边 */
	linkGraph->insertEdge(linkGraph, 0, 1, 4);
	linkGraph->insertEdge(linkGraph, 0, 7, 8);
	linkGraph->insertEdge(linkGraph, 1, 2, 8);
	linkGraph->insertEdge(linkGraph, 1, 7, 11);
	linkGraph->insertEdge(linkGraph, 2, 3, 7);
	linkGraph->insertEdge(linkGraph, 2, 5, 4);
	linkGraph->insertEdge(linkGraph, 2, 8, 2);
	linkGraph->insertEdge(linkGraph, 3, 4, 9);
	linkGraph->insertEdge(linkGraph, 3, 5, 14);
	linkGraph->insertEdge(linkGraph, 4, 5, 10);
	linkGraph->insertEdge(linkGraph, 5, 6, 2);
	linkGraph->insertEdge(linkGraph, 6, 7, 1);
	linkGraph->insertEdge(linkGraph, 6, 8, 6);
	/* 存放最小生成树 */
	loocLinkedGraph* MST_link = loocLinkedGraph_new(looc_file_line);
	MST_link->init(MST_link, linkGraph->numV, sizeof(int), linkGraph->check);
	/* 调用最小生成树算法Prim或者Kruskal */
	linkGraph->Kruskal(linkGraph, MST_link);
	/* 打印最小生成树 */
	for (i = 0; i < MST_link->numV; i++) {
		for (j = 0; j < MST_link->numV; j++) {
			int edgeValue = MST_link->getValueOfEdge(MST_link, i, j);
			if (edgeValue < LOOC_GRAPH_NO_EDGE) {
				printf("%d\t", edgeValue);
			} else {
				printf("-\t");
			}
		}
		printf("\r\n");
	}
	/* 释放图对象内存空间 */
	loocLinkedGraph_delete(linkGraph);
	loocLinkedGraph_delete(MST_link);
	/* 报告内存泄露情况 */
	looc_report();

	/* 最长递增子序列 */
	int X[9] = { 3, 6, 1, 4, 2, 8, 9, 5, 7 };
	int LIS_Res[9];
	int LIS_Path[9];
	int LIS_max = LIS(X, 9, LIS_Res, LIS_Path);
	printLIS(X, 9, LIS_Res, LIS_Path, LIS_max);

	/* 最长公共子序列/串 */
	int Y[9] = { 3, 6, 0, 2, 8, 9, 7, 1, 5 };
	int** LCS_Res = (int**) looc_malloc(10 * sizeof(int*), "LCS_Res",
	looc_file_line);
	int** LCS_Path = (int**) looc_malloc(10 * sizeof(int*), "LCS_Path",
	looc_file_line);
	for (i = 0; i < 10; i++) {
		LCS_Res[i] = (int*) looc_malloc(10 * sizeof(int), "LCS_Res[]",
		looc_file_line);
		LCS_Path[i] = (int*) looc_malloc(10 * sizeof(int), "LCS_Path[]",
		looc_file_line);
	}
	int LCS_max = LCS(X, Y, 9, 9, LCS_Res, LCS_Path, 0);
	printLCS(X, Y, 9, 9, LCS_Res, LCS_Path, LCS_max);
	for (i = 0; i < 10; i++) {
		looc_free(LCS_Res[i]);
		looc_free(LCS_Path[i]);
	}
	looc_free(LCS_Res);
	looc_free(LCS_Path);
	looc_report();

	/* 背包问题 */
	int weight[3] = { 3, 4, 5 };
	int price[3] = { 4, 5, 6 };
	int check[3];
	int maxValue = Knapsack(3, 10, weight, price, check);
	printf("Max Value:%d\r\n", maxValue);
	for (i = 0; i < 3; i++) {
		printf("%d\t", check[i]);
	}
	printf("\r\n");

	/* 字符串匹配 */
	char* T = "morrisandwendy";
	char* P = "nd";
	int position[10];
	int valid = RabinKarp(T, strlen(T), P, strlen(P), 7, position);
	printf("Rabin-Karp:");
	for (i = 0; i < valid; i++) {
		printf("%d\t", position[i]);
	}
	printf("\r\n");

	/* 最大公约数 */
	printf("%d和%d的最大公约数是%d\r\n", 12456, 32516, gcd(12456, 32516));

	/* 寻找指定范围内的所有素数 */
	int prime_result[100];
	printf("100以内总共 %d 个质数:\r\n", prime(100, prime_result));
	for (i = 0; i < 100; i++) {
		if (prime_result[i]) {
			printf("%d\t", prime_result[i]);
		}
	}
	printf("\r\n");

	/* 打印5阶幻方 */
	loocMatrix* magic = loocMatrix_magicMatrix(5);
	magic->print(magic);
	loocMatrix_delete(magic);
	looc_report();

	/* N皇后问题 */
	printf("%d皇后问题的解决方案共有%d\r\n", 8, loocMatrix_placeNQueen(8, 1));
	looc_report();

	/* 洗牌问题 */
	int card[52];
	shuffle(card, 52);
	for (i = 0; i < 52; i++) {
		switch ((card[i] - 1) / 13) {
		case 0:
			printf("黑桃");
			break;
		case 1:
			printf("红心");
			break;
		case 2:
			printf("方块");
			break;
		case 3:
			printf("梅花");
			break;
		}
		switch (card[i] % 13) {
		case 0:
			printf("K ");
			break;
		case 1:
			printf("A ");
			break;
		case 12:
			printf("Q ");
			break;
		case 11:
			printf("J ");
			break;
		default:
			printf("%d ", card[i] % 13);
			break;
		}
		if ((i + 1) % 13 == 0) {
			printf("\r\n");
		}
	}

	/* 约瑟夫问题 */
	int dieOrder[41];
	Josephus(41, 3, dieOrder);
	printf("死亡顺序：");
	for (i = 0; i < 41; i++) {
		printf("%d ", dieOrder[i]);
	}
	printf("\r\n");
	looc_report();

	/* 汉诺塔问题 */
	printf("移动汉诺塔中的%d个圆盘需要%d歩\r\n", 5, stepsOfHanoi(5, 'A', 'B', 'C', 1));

	/* 迭代法求平方根 */
	printf("71的平方根为:%lf(标准库)\t%lf(迭代法)\r\n", sqrt(71), SQR(71));

	/* 梯形法求定积分 */
	printf("定积分的值:%lf\r\n", DefiniteIntegration(expression, 0, 1));

	/* 整数划分 */
	printf("整数%d划分的方案数总共有%d中\r\n", 5, IntegerPart(5));

	/* 完美数 */
	int pfNumber[10];
	j = PerfectNumber(500, pfNumber);
	printf("500以内共有%d个完美数: ", j);
	for (i = 0; i < j; i++) {
		printf("%d\t", pfNumber[i]);
	}
	printf("\r\n");
	looc_report();

	/* 字符串 */
	loocString* lstr = loocString_new(looc_file_line);
	lstr->init(lstr, "  helloworld    worldhello  ");
	lstr->trim(lstr);
	lstr->print(lstr);
	/* 获取子字符串 */
	loocString* childstr = lstr->substr(lstr, 2, 8);
	childstr->print(childstr);
	/* 正序查找字符串 */
	printf("%d\r\n", lstr->find(lstr, "world"));
	/* 逆序查找字符串 */
	printf("%d\r\n", lstr->rfind(lstr, "world"));
	loocString_delete(childstr);
	loocString_delete(lstr);
	looc_report();

	/* 数组循环左移 */
	int shift_array[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	shift_left(shift_array, 10, 3);
	for (i = 0; i < 10; i++) {
		printf("%d ", shift_array[i]);
	}
	printf("\r\n");

	/* 寻找最大的N个数 */
	int origin_array[15] = { 5, 488, 21, 4554, 1, 33, 45, 64, 12, 14, 55, 74,
			12, 3, 123 };
	int result_array[5];
	maxN(origin_array, 15, 5, result_array);
	printf("最大的5个数是: ");
	for (i = 0; i < 5; i++) {
		printf("%d ", result_array[i]);
	}
	printf("\r\n");

	/* 寻找最大的N个数 */
	minN(origin_array, 15, 5, result_array);
	printf("最小的5个数是: ");
	for (i = 0; i < 5; i++) {
		printf("%d ", result_array[i]);
	}
	printf("\r\n");

	/* 查找数组中的最大最小值 */
	int maxmin_array[2];
	getMaxMin(origin_array, 15, maxmin_array);
	printf("最大值：%d\t最小值:%d\r\n", maxmin_array[0], maxmin_array[1]);

	/* 计算二进制1的个数 */
	printf("%d has %d '1'\r\n", 65530, countBinaryOne(65530));

	/* 计算表达式的值 */
	char express[100] = "1+9/3*2-1";
	printf("%s=%f\r\n", express, computeExpress(express));

	int A[] = { 0, 5, 5, 3, 5, 7, 5, 5 };
	int major = 0;
	if (findMajority(A, 8, &major) == 0) {
		printf("主元素为:%d\r\n", major);
	}

	/* 数字排列 */
	int Perm[] = { 1, 2, 3, 4 };
	Permutation(Perm, sizeof(Perm) / sizeof(Perm[0]), 0);
	Kth_Permutation(Perm, sizeof(Perm) / sizeof(Perm[0]), 6);

	/* 寻找和为s的两个数 */
	int f2s[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	findTwoSum(f2s, sizeof(f2s) / sizeof(f2s[0]), 10);

	/* 寻找和为s的整数序列 */
	int fss[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	findSequenceSum(fss, sizeof(fss) / sizeof(fss[0]), 21);

	/* 删除数组中重复的元素 */
	int rd[] = { 1, 2, 2, 2, 3, 3, 5, 5, 6, 6, 7, 8, 9, 9, 10, 10, 11, 13 };
	int rd_len = removeDuplicate(rd, sizeof(rd) / sizeof(rd[0]));
	for (int i = 0; i < rd_len; i++) {
		printf("%d ", rd[i]);
	}
	printf("\r\n");

	/* 字符串匹配 */
	char* pps = "aabbccagdbbccdec";
	char* ppp = "bbccd";
	printf("%d\r\n", strStr(pps, ppp));

	/* 指定压栈序列，判断弹栈序列是否合法 */
	int push_seq[] = { 1, 2, 3, 4, 5 };
	int pop_seq[] = { 4, 3, 5, 1, 2 };
	printf("%d\r\n",
			checkPopSequence(push_seq, pop_seq,
					sizeof(push_seq) / sizeof(push_seq[0])));

	/* 括号匹配 */
	printf("%d\r\n", isValidParentheses("{([]})"));

	/* 路径简化 */
	char sp[] = "/a/b/../c/../../d/./e/./f/g/";
	printf("%s\r\n", simplifyPath(sp));

	/*最大子列和*/
	int mss[] = { -3, 0, -2, -1, 1, 2, 3, -4 };
	int mss_start, mss_end;
	printf("最大子序列,和为：%d,",
			maxSubseqSum(mss, sizeof(mss) / sizeof(mss[0]), &mss_start,
					&mss_end));
	printf("开始下标：%d,结束下标:%d\r\n", mss_start, mss_end);

	/*最大矩形面积*/
	int mra[] = { 2, 1, 5, 6, 2, 3 };
	int mra_start, mra_end;
	printf("最大矩形面积：%d,",
			maxRectArea(mra, sizeof(mra) / sizeof(mra[0]), &mra_start,
					&mra_end));
	printf("开始下标：%d,结束下标:%d\r\n", mra_start, mra_end);

	return 0;
}
