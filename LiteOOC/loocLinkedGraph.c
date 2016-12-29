/*
 ============================================================================
 Name        : loocLinkedGraph.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 邻接表存储的图
 ============================================================================
 */

#include <loocLinkedGraph.h>
#include <loocQueue.h>
#include <string.h>

/**
 * 邻接链表中的每个节点的数据结构
 */
typedef struct AdjVNode_s {
	/* 邻接点下标 */
	int AdjIndex;
	/* 边权重 */
	int Weight;
} AdjVertexNode;

/**
 * 图的初始化
 * @param cthis       当前图对象指针
 * @param maxVertex   最大顶点数
 * @param elementSize 顶点数据大小
 * @param iCheck      是否是有向图
 */
static void loocLinkedGraph_init(loocLinkedGraph* cthis, int maxVertex,
		int elementSize, int icheck) {
	int i;
	cthis->_elementSize = elementSize;
	cthis->_maxVertex = maxVertex;
	cthis->check = icheck;
	cthis->data_pool = looc_malloc(elementSize * maxVertex,
			"loocLinkedGraph_data",
			looc_file_line);
	cthis->h = (loocSingleList**) looc_malloc(
			sizeof(loocSingleList*) * maxVertex, "loocLinkedGraph_head",
			looc_file_line);
	for (i = 0; i < maxVertex; i++) {
		cthis->h[i] = NULL;
	}
}

/**
 * 向图中添加顶点
 * @param  cthis 当前图对象指针
 * @param  data  顶点数据指针
 * @return       成功返回true，失败返回false
 */
static looc_bool loocLinkedGraph_addVertex(loocLinkedGraph* cthis, void* data) {
	/* 判断是否已达顶点上限 */
	if (cthis->numV >= cthis->_maxVertex || data == NULL) {
		return looc_false;
	}
	/* 将数据拷贝至顶点中 */
	memcpy(cthis->data_pool + cthis->numV * cthis->_elementSize, data,
			cthis->_elementSize);
	cthis->numV++;
	return looc_true;
}

/**
 * 向图中添加边
 * @param  cthis  当前图对象指针
 * @param  v1     边的一点
 * @param  v2     边的另一点
 * @param  weight 边的权重
 * @return        成功返回true，失败返回false
 */
static looc_bool loocLinkedGraph_insertEdge(loocLinkedGraph* cthis, int v1,
		int v2, int weight) {
	AdjVertexNode node;
	loocSingleListNode* listNode;
	if ((v1 >= cthis->numV) || (v2 >= cthis->numV) || (v1 < 0) || (v2 < 0)) {
		return looc_false;
	}
	/* 插入边 <V1, V2> */
	/* 还没有该顶点参与的边 */
	if (cthis->h[v1] == NULL) {
		cthis->h[v1] = loocSingleList_new(looc_file_line);
		cthis->h[v1]->init(cthis->h[v1], sizeof(node), NULL);
	}
	/* 相同的边是否已经存在 */
	listNode = cthis->h[v1]->head;
	while (listNode) {
		if (((AdjVertexNode*) (listNode->_data))->AdjIndex == v2) {
			((AdjVertexNode*) (listNode->_data))->Weight = weight;
			break;
		}
		listNode = listNode->next;
	}
	/* 不存在相同的边 */
	if (listNode == NULL) {
		node.AdjIndex = v2;
		node.Weight = weight;
		cthis->h[v1]->insertAt(cthis->h[v1], 0, (void*) &node);
	}
	/* 边数加1 */
	cthis->numE++;
	/* 若是无向图，还要插入边 <V2, V1> */
	if (cthis->check == 0) {
		if (cthis->h[v2] == NULL) {
			cthis->h[v2] = loocSingleList_new(looc_file_line);
			cthis->h[v2]->init(cthis->h[v2], sizeof(node), NULL);
		}
		listNode = cthis->h[v2]->head;
		while (listNode) {
			if (((AdjVertexNode*) (listNode->_data))->AdjIndex == v1) {
				((AdjVertexNode*) (listNode->_data))->Weight = weight;
				break;
			}
			listNode = listNode->next;
		}
		if (listNode == NULL) {
			node.AdjIndex = v1;
			node.Weight = weight;
			cthis->h[v2]->insertAt(cthis->h[v2], 0, (void*) &node);
		}
	}
	return looc_true;
}

/**
 * 从图中删除边
 * @param  cthis  当前图对象指针
 * @param  v1     边的一点
 * @param  v2     边的另一点
 * @return        成功返回true，失败返回false
 */
static looc_bool loocLinkedGraph_deleteEdge(loocLinkedGraph* cthis, int v1,
		int v2) {
	loocSingleListNode* listNode;
	AdjVertexNode* node;
	int position = -1;
	if ((v1 >= cthis->numV) || (v2 >= cthis->numV) || (v1 < 0) || (v2 < 0)) {
		return looc_false;
	}
	/* 删除边<v1,v2> */
	listNode = cthis->h[v1]->head;
	while (listNode) {
		node = (AdjVertexNode*) (listNode->_data);
		position++;
		if (node->AdjIndex == v2) {
			cthis->h[v1]->removeAt(cthis->h[v1], position);
			cthis->numE--;
			break;
		}
		listNode = listNode->next;
	}
	if (listNode == NULL) {
		return looc_false;
	}
	/* 如果是无向图，删除边<v2,v1> */
	if (cthis->check == 0) {
		listNode = cthis->h[v2]->head;
		position = -1;
		while (listNode) {
			node = (AdjVertexNode*) (listNode->_data);
			position++;
			if (node->AdjIndex == v1) {
				cthis->h[v2]->removeAt(cthis->h[v2], position);
				break;
			}
			listNode = listNode->next;
		}
		if (listNode == NULL) {
			return looc_false;
		}
	}
	return looc_true;
}

/**
 * 判定顶点之间是否有边存在
 * @param  cthis 当前图对象指针
 * @param  v1    顶点v1
 * @param  v2    顶点v2
 * @return       存在返回true，不存在返回false
 */
static looc_bool loocLinkedGraph_existEdge(loocLinkedGraph* cthis, int v1,
		int v2) {
	loocSingleListNode* listNode;
	AdjVertexNode* node;
	if ((cthis->numV <= v1) || (cthis->numV <= v2) || (v1 < 0) || (v2 < 0)) {
		return looc_false;
	} else {
		listNode = cthis->h[v1]->head;
		while (listNode) {
			node = (AdjVertexNode*) (listNode->_data);
			if (node->AdjIndex == v2) {
				return looc_true;
			}
		}
		return looc_false;
	}
}

/**
 * 计算顶点的出度
 * @param  cthis 当前图对象指针
 * @param  v     某个顶点的索引
 * @return       返回指定顶点的出度
 */
static int loocLinkedGraph_outDegree(loocLinkedGraph* cthis, int v) {
	if ((cthis->numV <= v) || (v < 0)) {
		return 0;
	} else {
		/* 就是计算节点的边链表的长度 */
		return cthis->h[v]->length;
	}
}

/**
 * 计算顶点的入度
 * @param  cthis 当前图对象指针
 * @param  v     某个顶点的索引
 * @return       返回指定顶点的入度
 */
static int loocLinkedGraph_inDegree(loocLinkedGraph* cthis, int v) {
	int sum = 0;
	int i;
	loocSingleListNode* listNode;
	AdjVertexNode* node;
	if ((cthis->numV <= v) || (v < 0)) {
		sum = 0;
	} else {
		for (i = 0; i < cthis->numV; i++) {
			if (i != v) {
				listNode = cthis->h[i]->head;
				while (listNode) {
					node = (AdjVertexNode*) (listNode->_data);
					if (node->AdjIndex == v) {
						sum++;
					}
					listNode = listNode->next;
				}
			}
		}
	}
	return sum;
}

/**
 * 真正的dfs算法
 * @param cthis 当前图对象指针
 * @param v     开始访问的第一个顶点
 * @param action 访问函数，由用户自己实现
 * @param args	传递给action函数的参数
 * @param visited 访问标记数组
 */
static void dfs(loocLinkedGraph* cthis, int v,
		void (*action)(void* node, void* args), void* args, int* visited) {
	loocSingleListNode* listNode;
	AdjVertexNode* node;
	/* 访问当前节点 */
	action(cthis->data_pool + cthis->_elementSize * v, args);
	visited[v] = 1;
	listNode = cthis->h[v]->head;
	/* 对v的每个邻接点递归访问 */
	while (listNode) {
		node = (AdjVertexNode*) (listNode->_data);
		if (visited[node->AdjIndex] == 0) {
			dfs(cthis, node->AdjIndex, action, args, visited);
		}
		listNode = listNode->next;
	}
}

/**
 * 图的深度优先遍历
 * @param cthis 当前图对象指针
 * @param v     开始访问的第一个顶点
 * @param action 访问函数，由用户自己实现
 * @param args	传递给action函数的参数
 */
static void loocLinkedGraph_DFS(loocLinkedGraph* cthis, int v,
		void (*action)(void* node, void* args), void* args) {
	int* visited = looc_malloc(sizeof(int) * cthis->numV,
			"loocLinkedGraph_visited", looc_file_line);
	int i = 0;
	/* 初始化访问数组 */
	for (i = 0; i < cthis->numV; i++) {
		visited[i] = 0;
	}
	/* 调用dfs算法 */
	dfs(cthis, v, action, args, visited);
	/* 释放访问数组空间 */
	looc_free(visited);
}

/**
 * 图的广度优先遍历
 * @param cthis 当前图对象指针
 * @param v     开始访问的第一个顶点
 * @param action 访问函数，由用户自己实现
 * @param args	传递给action函数的参数
 */
static void loocLinkedGraph_BFS(loocLinkedGraph* cthis, int v,
		void (*action)(void* node, void* args), void* args) {
	int* visited = looc_malloc(sizeof(int) * cthis->numV,
			"loocLinkedGraph_visited", looc_file_line);
	int i = 0;
	loocSingleListNode* listNode;
	AdjVertexNode* node;
	/* 设置队列，记录访问顺序 */
	loocQueue* queue = loocQueue_new(looc_file_line);
	queue->init(queue, cthis->numV, sizeof(int));
	/* 初始化访问数组 */
	for (i = 0; i < cthis->numV; i++) {
		visited[i] = 0;
	}
	queue->enqueue(queue, (void*) &v);
	/* 队列不为空，循环遍历 */
	while (queue->length) {
		v = *(int*) queue->dequeue(queue);
		if (visited[v] == 0) {
			/* 访问当前节点 */
			action(cthis->data_pool + cthis->_elementSize * v, args);
			visited[v] = 1;
			listNode = cthis->h[v]->head;
			while (listNode) {
				node = (AdjVertexNode*) (listNode->_data);
				if (visited[node->AdjIndex] == 0) {
					queue->enqueue(queue, (void*) &(node->AdjIndex));
				}
				listNode = listNode->next;
			}
		}
	}
	/* 释放队列的内存空间 */
	loocQueue_delete(queue);
	/* 释放访问数组空间 */
	looc_free(visited);
}

/**
 * 图的销毁函数
 * @param object loocObject对象指针
 */
static void loocLinkedGraph_finalize(loocObject* object) {
	int i;
	loocLinkedGraph* graph = SUB_PTR(object, loocObject, loocLinkedGraph);
	if (graph->data_pool) {
		looc_free(graph->data_pool);
	}
	if (graph->h) {
		for (i = 0; i < graph->numV; i++) {
			if (graph->h[i]) {
				loocSingleList_delete(graph->h[i]);
			}
		}
		looc_free(graph->h);
	}
	graph->numE = 0;
	graph->numV = 0;
}

/**
 * loocLinkedGraph的构造函数
 */
CTOR(loocLinkedGraph)
/* 调用父类构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_elementSize = 1;
	cthis->numV = 0;
	cthis->numE = 0;
	cthis->h = NULL;
	cthis->check = 0;
	/* 成员函数绑定 */
	FUNCTION_SETTING(init, loocLinkedGraph_init);
	FUNCTION_SETTING(addVertex, loocLinkedGraph_addVertex);
	FUNCTION_SETTING(insertEdge, loocLinkedGraph_insertEdge);
	FUNCTION_SETTING(deleteEdge, loocLinkedGraph_deleteEdge);
	FUNCTION_SETTING(existEdge, loocLinkedGraph_existEdge);
	FUNCTION_SETTING(outDegree, loocLinkedGraph_outDegree);
	FUNCTION_SETTING(inDegree, loocLinkedGraph_inDegree);
	FUNCTION_SETTING(DFS, loocLinkedGraph_DFS);
	FUNCTION_SETTING(BFS, loocLinkedGraph_BFS);
	FUNCTION_SETTING(loocObject.finalize, loocLinkedGraph_finalize);END_CTOR

/**
 * loocLinkedGraph的析构函数
 */
DTOR(loocLinkedGraph)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
