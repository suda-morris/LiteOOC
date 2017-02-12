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
#include <loocHeap.h>
#include <loocDisjointSet.h>
#include <string.h>

/**
 * 邻接链表中的每个邻接节点的数据结构
 */
typedef struct AdjVNode_s {
	/* 邻接点下标 */
	int AdjIndex;
	/* 边权重 */
	int Weight;
} AdjVertexNode;

/**
 * 保存每一条边的权值和顶点,v1->v2
 */
typedef struct Edge_s {
	int v1;
	int v2;
	int weight;
} Edge;

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
		cthis->h[i] = loocSingleList_new(looc_file_line);
		cthis->h[i]->init(cthis->h[i], sizeof(AdjVertexNode), NULL);
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
	if ((v1 >= cthis->numV) || (v2 >= cthis->numV) || (v1 < 0) || (v2 < 0)
			|| (v1 == v2)) {
		return looc_false;
	}
	/* 插入边 <V1, V2> */
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
	if ((v1 >= cthis->numV) || (v2 >= cthis->numV) || (v1 < 0) || (v2 < 0)
			|| (v1 == v2)) {
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
	if ((cthis->numV <= v1) || (cthis->numV <= v2) || (v1 < 0) || (v2 < 0)
			|| (v1 == v2)) {
		return looc_false;
	} else {
		listNode = cthis->h[v1]->head;
		while (listNode) {
			node = (AdjVertexNode*) (listNode->_data);
			if (node->AdjIndex == v2) {
				return looc_true;
			}
			listNode = listNode->next;
		}
		return looc_false;
	}
}

/**
 * 获取边的权值
 * @param  cthis 当前图对象指针
 * @param  v1    顶点v1
 * @param  v2    顶点v2
 * @return       返回权值
 */
static int loocLinkedGraph_getValueOfEdge(loocLinkedGraph* cthis, int v1,
		int v2) {
	loocSingleListNode* listNode;
	AdjVertexNode* node;
	if ((cthis->numV <= v1) || (cthis->numV <= v2) || (v1 < 0) || (v2 < 0)) {
		return LOOC_GRAPH_NO_EDGE;
	} else if (v1 == v2) {
		return 0;
	} else {
		listNode = cthis->h[v1]->head;
		while (listNode) {
			node = (AdjVertexNode*) (listNode->_data);
			if (node->AdjIndex == v2) {
				return node->Weight;
			}
			listNode = listNode->next;
		}
		return LOOC_GRAPH_NO_EDGE;
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
 * 拓扑排序
 * @param  cthis 当前图对象指针
 * @param  order 存储排序后的顶点下标
 * @return       成功返回true，否则返回false(表明这不是一个DAG，即不是一个有向无环图)
 */
static looc_bool loocLinkedGraph_topologySort(loocLinkedGraph* cthis,
		int order[]) {
	int indegree[cthis->_maxVertex];
	int i, v;
	int cnt = 0;
	loocSingleListNode* listNode = NULL;
	AdjVertexNode* node = NULL;
	loocQueue* queue = NULL;
	if (cthis->check) {
		queue = loocQueue_new(looc_file_line);
		queue->init(queue, cthis->_maxVertex, sizeof(int));
		for (i = 0; i < cthis->numV; i++) {
			indegree[i] = 0;
		}
		/* 遍历图得到每个顶点的入度 */
		for (i = 0; i < cthis->numV; i++) {
			listNode = cthis->h[i]->head;
			while (listNode) {
				node = (AdjVertexNode*) (listNode->_data);
				indegree[node->AdjIndex]++;
				listNode = listNode->next;
			}
		}
		/* 将所有入度为0的顶点入列 */
		for (i = 0; i < cthis->numV; i++) {
			if (indegree[i] == 0) {
				queue->enqueue(queue, (void*) &i);
			}
		}
		/* 开始拓扑排序 */
		while (queue->length) {
			v = *(int*) queue->dequeue(queue);
			order[cnt++] = v;
			listNode = cthis->h[v]->head;
			while (listNode) {
				node = (AdjVertexNode*) (listNode->_data);
				if (--indegree[node->AdjIndex] == 0) {
					queue->enqueue(queue, (void*) &(node->AdjIndex));
				}
				listNode = listNode->next;
			}
		}
		loocQueue_delete(queue);
		if (cnt != cthis->numV) {
			return looc_false;
		} else {
			return looc_true;
		}
	} else {
		return looc_false;
	}
}

/**
 * 单源最短路径
 * @param  cthis 当前图对象指针
 * @param  S     源点
 * @param  dist  各顶点到源的最短路径距离
 * @param  path  记录最短路径上中间点的前驱点
 * @return       成功返回true，失败返回false
 */
static looc_bool loocLinkedGraph_Dijkstra(loocLinkedGraph* cthis, int S,
		int dist[], int path[]) {
	int v;
	int minV;
	int minDist;
	int* collected = looc_malloc(sizeof(int) * cthis->numV,
			"loocLinkedGraph_collected", looc_file_line);
	loocSingleListNode* listNode;
	AdjVertexNode* node;
	/* 初始化 */
	for (v = 0; v < cthis->numV; v++) {
		dist[v] = LOOC_GRAPH_NO_EDGE;
		collected[v] = 0;
		path[v] = -1;
	}
	listNode = cthis->h[S]->head;
	while (listNode) {
		node = (AdjVertexNode*) (listNode->_data);
		/* 无法处理负数的权值 */
		if (node->Weight < 0) {
			return looc_false;
		}
		dist[node->AdjIndex] = node->Weight;
		path[node->AdjIndex] = S;
		listNode = listNode->next;
	}
	/* 将起点纳入集合中 */
	collected[S] = 1;
	dist[S] = 0;
	/* 将未放入集合中的点放入集合，选取的标准是dist[]最小 */
	while (1) {
		/* 寻找最小的dist */
		minV = S;
		minDist = LOOC_GRAPH_NO_EDGE;
		for (v = 0; v < cthis->numV; v++) {
			if (collected[v] == 0 && dist[v] < minDist) {
				minV = v;
				minDist = dist[v];
			}
		}
		/* 如果全部的点都已经收录 */
		if (minDist == LOOC_GRAPH_NO_EDGE) {
			break;
		}
		/* 收录minV */
		collected[minV] = 1;
		/* 更新dist */
		listNode = cthis->h[minV]->head;
		while (listNode) {
			node = (AdjVertexNode*) (listNode->_data);
			if (node->Weight < 0) {
				return looc_false;	//无法处理负数的权值
			}
			if (dist[node->AdjIndex] > dist[minV] + node->Weight) {
				dist[node->AdjIndex] = dist[minV] + node->Weight;
				path[node->AdjIndex] = minV;
			}
			listNode = listNode->next;
		}
	}
	/* 释放collected */
	looc_free(collected);
	return looc_true;
}

/**
 * 多源最短路径
 * @param  cthis 当前图对象指针
 * @param  D     保存每对顶点的最短路径的权值
 * @param  path  记录最短路径上中间点的前驱点
 * @return       成功返回true，失败返回false
 */
static looc_bool loocLinkedGraph_Floyd(loocLinkedGraph* cthis,
		int D[][LOOC_DEFAULT_LOOCLINKEDGRAPH_VERTEX],
		int path[][LOOC_DEFAULT_LOOCLINKEDGRAPH_VERTEX]) {
	int i, j, k;
	loocSingleListNode* listNode;
	AdjVertexNode* node;
	/* 初始化 */
	for (i = 0; i < cthis->_maxVertex; i++) {
		for (j = 0; j < cthis->_maxVertex; j++) {
			if (i == j) {
				D[i][j] = 0;
			} else {
				D[i][j] = LOOC_GRAPH_NO_EDGE;
			}
			path[i][j] = -1;
		}
	}
	for (i = 0; i < cthis->numV; i++) {
		listNode = cthis->h[i]->head;
		while (listNode) {
			node = (AdjVertexNode*) (listNode->_data);
			D[i][node->AdjIndex] = node->Weight;
			listNode = listNode->next;
		}
	}
	/* Floyd核心算法 */
	for (k = 0; k < cthis->numV; k++)
		for (i = 0; i < cthis->numV; i++)
			for (j = 0; j < cthis->numV; j++)
				if (D[i][k] + D[k][j] < D[i][j]) {
					D[i][j] = D[i][k] + D[k][j];
					if (i == j && D[i][j] < 0) //若发现负值圈
						return looc_false; //不能正确解决，返回错误标记
					path[i][j] = k;	//i到j的最短路径必须要经过k
				}
	return looc_true;
}

/**
 * 最小生成树Prim算法
 * @param  cthis 当前图对象指针
 * @param  MST   保存最小生成树
 * @return       成功返回最小生成树的权值和，失败(非连通树)返回-1
 */
static int loocLinkedGraph_Prim(loocLinkedGraph* cthis, loocLinkedGraph* MST) {
	int* dist = looc_malloc(sizeof(int) * cthis->_maxVertex,
			"loocAdjacencyGraph_dist", looc_file_line);
	int* parent = looc_malloc(sizeof(int) * cthis->_maxVertex,
			"loocAdjacencyGraph_parent", looc_file_line);
	int i;
	int minV, minDist;
	int VCount;
	int totalWeight;
	loocSingleListNode* listNode;
	AdjVertexNode* node;
	/* 初始化，默认初始点下标是0 */
	for (i = 0; i < cthis->numV; i++) {
		dist[i] = LOOC_GRAPH_NO_EDGE;
		parent[i] = 0;	//暂且定义所有顶点的父节点都是初始点0
		MST->addVertex(MST, cthis->data_pool + i * cthis->_elementSize);
	}
	listNode = cthis->h[0]->head;
	while (listNode) {
		node = (AdjVertexNode*) (listNode->_data);
		dist[node->AdjIndex] = node->Weight;
		listNode = listNode->next;
	}
	VCount = 0;
	totalWeight = 0;
	/* 将初始点0收录进MST */
	dist[0] = 0;
	VCount++;
	parent[0] = -1;	//当前树的根是0
	while (1) {
		/* 寻找未收录顶点中dist最小者 */
		minDist = LOOC_GRAPH_NO_EDGE;
		for (i = 0; i < cthis->numV; i++) {
			if (dist[i] != 0 && dist[i] < minDist) {
				minDist = dist[i];
				minV = i;
			}
		}
		/* 未找到最小dist */
		if (minDist == LOOC_GRAPH_NO_EDGE) {
			break;
		}
		/* 将minV以及相应的边<parent[minV],minV>收录进MST */
		MST->insertEdge(MST, parent[minV], minV, dist[minV]);
		dist[minV] = 0;
		VCount++;
		totalWeight += dist[minV];
		/* 更新dist和parent */
		listNode = cthis->h[minV]->head;
		while (listNode) {
			node = (AdjVertexNode*) (listNode->_data);
			/* 若是minV的邻接点且未被收录 */
			if ((dist[node->AdjIndex] != 0)
					&& (node->Weight < dist[node->AdjIndex])) {
				dist[node->AdjIndex] = node->Weight;
				parent[node->AdjIndex] = minV;
			}
			listNode = listNode->next;
		}
	}
	/* 释放临时资源 */
	looc_free(dist);
	looc_free(parent);
	if (VCount < cthis->numV) {
		return -1;	//此树非连通
	}
	return totalWeight;
}

static int Heap_compareStrategy(void* old, void* new) {
	Edge* node1 = (Edge*) old;
	Edge* node2 = (Edge*) new;
	if (node1->weight > node2->weight) {
		return -1;
	} else if (node1->weight < node2->weight) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * 最小生成树Kruskal算法
 * @param  cthis 当前图对象指针
 * @param  MST   保存最小生成树
 * @return       成功返回最小生成树的权值和，失败(非连通树)返回-1
 */
static int loocLinkedGraph_Kruskal(loocLinkedGraph* cthis, loocLinkedGraph* MST) {
	int totalWeight = 0;
	int ECount = 0;	//收录的边数
	int i;
	int root1, root2;
	Edge e;
	loocSingleListNode* listNode;
	AdjVertexNode* node;
	/* 最小堆，保存每一条边 */
	loocHeap* heap = loocHeap_new(looc_file_line);
	heap->init(heap, LOOC_MIN_HEAP, cthis->numE, sizeof(Edge),
			Heap_compareStrategy);
	/* 并查集，保存顶点下标 */
	loocDisjointSet* set = loocDisjointSet_new(looc_file_line);
	set->init(set, cthis->numV, sizeof(int));
	for (i = 0; i < cthis->numV; i++) {
		listNode = cthis->h[i]->head;
		while (listNode) {
			node = (AdjVertexNode*) (listNode->_data);
			/* 避免无向图中重复录入边 */
			if ((cthis->check == 1)
					|| ((cthis->check == 0) && (i < node->AdjIndex))) {
				e.v1 = i;
				e.v2 = node->AdjIndex;
				e.weight = node->Weight;
				heap->insert(heap, (void*) &e);
			}
			listNode = listNode->next;
		}
		/* MST包含所有顶点 */
		MST->addVertex(MST, cthis->data_pool + i * cthis->_elementSize);
		set->insert(set, (void*) &i);
	}
	while (ECount < cthis->numV - 1) {
		if (heap->getRoot(heap)) {
			e = *(Edge*) heap->getRoot(heap);
			root1 = set->find(set, &e.v1);
			root2 = set->find(set, &e.v2);
			/* 不能构成回路 */
			if (root1 != root2) {
				/* 将该边插入MST */
				MST->insertEdge(MST, e.v1, e.v2, e.weight);
				totalWeight += e.weight;
				ECount++;
				/* 并操作 */
				set->Union(set, &e.v1, &e.v2);
			}
			heap->deleteRoot(heap);
		} else {
			break;	//边集已空
		}
	}
	/* 释放临时内存 */
	loocHeap_delete(heap);
	loocDisjointSet_delete(set);
	if (ECount < cthis->numV - 1) {
		return -1;	//此树不连通
	}
	return totalWeight;
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
		for (i = 0; i < graph->_maxVertex; i++) {
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
	FUNCTION_SETTING(getValueOfEdge, loocLinkedGraph_getValueOfEdge);
	FUNCTION_SETTING(outDegree, loocLinkedGraph_outDegree);
	FUNCTION_SETTING(inDegree, loocLinkedGraph_inDegree);
	FUNCTION_SETTING(DFS, loocLinkedGraph_DFS);
	FUNCTION_SETTING(BFS, loocLinkedGraph_BFS);
	FUNCTION_SETTING(topologySort, loocLinkedGraph_topologySort);
	FUNCTION_SETTING(Dijkstra, loocLinkedGraph_Dijkstra);
	FUNCTION_SETTING(Floyd, loocLinkedGraph_Floyd);
	FUNCTION_SETTING(Prim, loocLinkedGraph_Prim);
	FUNCTION_SETTING(Kruskal, loocLinkedGraph_Kruskal);
	FUNCTION_SETTING(loocObject.finalize, loocLinkedGraph_finalize);END_CTOR

/**
 * loocLinkedGraph的析构函数
 */
DTOR(loocLinkedGraph)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
