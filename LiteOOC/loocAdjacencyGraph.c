/*
 ============================================================================
 Name        : loocAdjacencyGraph.c
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 邻接矩阵存储的图
 ============================================================================
 */

#include <loocAdjacencyGraph.h>
#include <loocQueue.h>
#include <loocHeap.h>
#include <loocDisjointSet.h>
#include <string.h>
#include <stdio.h>

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
static void loocAdjacencyGraph_init(loocAdjacencyGraph* cthis, int maxVertex,
		int elementSize, int iCheck) {
	int v, w;
	cthis->check = iCheck;
	cthis->_elementSize = elementSize;
	cthis->_maxVertex = maxVertex;
	cthis->data_pool = looc_malloc(elementSize * maxVertex,
			"loocAdjacencyGraph_data",
			looc_file_line);
	cthis->G = (int*) looc_malloc(sizeof(int) * maxVertex * maxVertex,
			"loocAdjacency_G",
			looc_file_line);
	/* 初始化邻接矩阵 */
	for (v = 0; v < maxVertex; v++) {
		for (w = 0; w < maxVertex; w++) {
			if (v == w) {
				*(cthis->G + v * maxVertex + w) = 0;	//邻接矩阵主对角线为0
			} else {
				*(cthis->G + v * maxVertex + w) = LOOC_GRAPH_NO_EDGE;
			}
		}
	}
}

/**
 * 向图中添加边
 * @param  cthis  当前图对象指针
 * @param  v1     边的一点
 * @param  v2     边的另一点
 * @param  weight 边的权重
 * @return        成功返回true，失败返回false
 */
static looc_bool loocAdjacencyGraph_insertEdge(loocAdjacencyGraph* cthis,
		int v1, int v2, int weight) {
	if ((v1 >= cthis->numV) || (v2 >= cthis->numV) || (v1 < 0) || (v2 < 0)
			|| (v1 == v2)) {
		return looc_false;
	}
	/* 插入边<v1,v2> */
	*(cthis->G + v1 * cthis->_maxVertex + v2) = weight;
	cthis->numE++;
	/* 如果是无向图，还要插入边<v2,v1> */
	if (cthis->check == 0) {
		*(cthis->G + v2 * cthis->_maxVertex + v1) = weight;
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
static looc_bool loocAdjacencyGraph_deleteEdge(loocAdjacencyGraph* cthis,
		int v1, int v2) {
	if ((v1 >= cthis->numV) || (v2 >= cthis->numV) || (v1 < 0) || (v2 < 0)
			|| (v1 == v2)) {
		return looc_false;
	}
	/* 删除边<v1,v2> */
	*(cthis->G + v1 * cthis->_maxVertex + v2) = LOOC_GRAPH_NO_EDGE;
	cthis->numE--;
	/* 如果是无向图，还要删除边<v2,v1> */
	if (cthis->check == 0) {
		*(cthis->G + v2 * cthis->_maxVertex + v1) = LOOC_GRAPH_NO_EDGE;
	}
	return looc_true;
}

/**
 * 向图中添加顶点
 * @param  cthis 当前图对象指针
 * @param  data  顶点数据指针
 * @return       成功返回true，失败返回false
 */
static looc_bool loocAdjacencyGraph_addVertex(loocAdjacencyGraph* cthis,
		void* data) {
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
 * 判定顶点之间是否有边存在
 * @param  cthis 当前图对象指针
 * @param  v1    顶点v1
 * @param  v2    顶点v2
 * @return       存在返回true，不存在返回false
 */
static looc_bool loocAdjacencyGraph_existEdge(loocAdjacencyGraph* cthis, int v1,
		int v2) {
	if ((cthis->numV <= v1) || (cthis->numV <= v2) || (v1 < 0) || (v2 < 0)
			|| (v1 == v2)
			|| ((*(cthis->G + v1 * cthis->_maxVertex + v2))
					== LOOC_GRAPH_NO_EDGE)) {
		return looc_false;
	} else {
		return looc_true;
	}
}

/**
 * 获取边的权值
 * @param  cthis 当前图对象指针
 * @param  v1    顶点v1
 * @param  v2    顶点v2
 * @return       返回权值
 */
static int loocAdjacencyGraph_getValueOfEdge(loocAdjacencyGraph* cthis, int v1,
		int v2) {
	if ((cthis->numV <= v1) || (cthis->numV <= v2) || (v1 < 0) || (v2 < 0)) {
		return LOOC_GRAPH_NO_EDGE;
	} else {
		return *(cthis->G + v1 * cthis->_maxVertex + v2);
	}
}

/**
 * 计算顶点的出度
 * @param  cthis 当前图对象指针
 * @param  v     某个顶点的索引
 * @return       返回指定顶点的出度
 */
static int loocAdjacencyGraph_outDegree(loocAdjacencyGraph* cthis, int v) {
	int sum = 0;
	int i;
	if ((cthis->numV <= v) || (v < 0)) {
		sum = 0;
	} else {
		/* 就是计算G[v]这一行有边的元素个数 */
		for (i = 0; i < cthis->numV; i++) {
			if ((i != v)
					&& ((*(cthis->G + v * cthis->_maxVertex + i))
							!= LOOC_GRAPH_NO_EDGE)) {
				sum++;
			}
		}
	}
	return sum;
}

/**
 * 计算顶点的入度
 * @param  cthis 当前图对象指针
 * @param  v     某个顶点的索引
 * @return       返回指定顶点的入度
 */
static int loocAdjacencyGraph_inDegree(loocAdjacencyGraph* cthis, int v) {
	int sum = 0;
	int i;
	if ((cthis->numV <= v) || (v < 0)) {
		sum = 0;
	} else {
		/* 就是计算G[v]这一列有边的元素个数 */
		for (i = 0; i < cthis->numV; i++) {
			if ((i != v)
					&& ((*(cthis->G + i * cthis->_maxVertex + v))
							!= LOOC_GRAPH_NO_EDGE)) {
				sum++;
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
static void dfs(loocAdjacencyGraph* cthis, int v,
		void (*action)(void* node, void* args), void* args, int* visited) {
	int i;
	/* 访问当前节点 */
	action(cthis->data_pool + cthis->_elementSize * v, args);
	visited[v] = 1;
	for (i = 0; i < cthis->numV; i++) {
		if ((i != v)
				&& ((*(cthis->G + v * cthis->_maxVertex + i))
						!= LOOC_GRAPH_NO_EDGE)) {
			if (visited[i] == 0) {
				dfs(cthis, i, action, args, visited);
			}
		}
	}
}

/**
 * 图的深度优先遍历
 * @param cthis 当前图对象指针
 * @param v     开始访问的第一个顶点
 * @param action 访问函数，由用户自己实现
 * @param args	传递给action函数的参数
 */
static void loocAdjacencyGraph_DFS(loocAdjacencyGraph* cthis, int v,
		void (*action)(void* node, void* args), void* args) {
	int* visited = looc_malloc(sizeof(int) * cthis->numV,
			"loocAdjacencyGraph_visited", looc_file_line);
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
static void loocAdjacencyGraph_BFS(loocAdjacencyGraph* cthis, int v,
		void (*action)(void* node, void* args), void* args) {
	int* visited = looc_malloc(sizeof(int) * cthis->numV,
			"loocAdjacencyGraph_visited", looc_file_line);
	int i = 0;
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
			for (i = 0; i < cthis->numV; i++) {
				if (loocAdjacencyGraph_existEdge(cthis, v, i)) {
					if (visited[i] == 0) {
						queue->enqueue(queue, (void*) &i);
					}
				}
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
static looc_bool loocAdjacencyGraph_topologySort(loocAdjacencyGraph* cthis,
		int order[]) {
	int indegree[cthis->_maxVertex];
	int i, j, v;
	int cnt = 0;
	loocQueue* queue = NULL;
	if (cthis->check) {
		queue = loocQueue_new(looc_file_line);
		queue->init(queue, cthis->_maxVertex, sizeof(int));
		for (i = 0; i < cthis->numV; i++) {
			indegree[i] = 0;
		}
		/* 遍历图得到每个顶点的入度 */
		for (i = 0; i < cthis->numV; i++) {
			for (j = 0; j < cthis->numV; j++) {
				if ((i != j)
						&& ((*(cthis->G + i * cthis->_maxVertex + j))
								!= LOOC_GRAPH_NO_EDGE)) {
					indegree[j]++;
				}
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
			for (i = 0; i < cthis->numV; i++) {
				if ((i != v)
						&& ((*(cthis->G + v * cthis->_maxVertex + i))
								!= LOOC_GRAPH_NO_EDGE)) {
					if (--indegree[i] == 0) {
						queue->enqueue(queue, (void*) &i);
					}
				}
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
static looc_bool loocAdjacencyGraph_Dijkstra(loocAdjacencyGraph* cthis, int S,
		int dist[], int path[]) {
	int v, minV;
	int minDist;
	int* collected = looc_malloc(sizeof(int) * cthis->numV,
			"loocAdjacencyGraph_collected", looc_file_line);
	/* 初始化 */
	for (v = 0; v < cthis->numV; v++) {
		/* 获取图中S到各顶点的权值作为dist初始值 */
		dist[v] = *(cthis->G + S * cthis->_maxVertex + v);
		if (dist[v] < 0) {
			return looc_false;	//无法处理负数的权值
		}
		collected[v] = 0;
		if (dist[v] == LOOC_GRAPH_NO_EDGE) {
			path[v] = -1;	//如果S和v之间无边，则赋值-1
		} else {
			path[v] = S;	//S和v之间有边，则前驱点初始化为S
		}
	}
	/* 先将起点收入集合 */
	dist[S] = 0;
	collected[S] = 1;
	/* 将未放入集合中的点放入集合，选取的标准是dist[]最小 */
	while (1) {
		/* minV = 未被收录顶点中dist最小者 */
		minDist = LOOC_GRAPH_NO_EDGE;
		minV = S;
		for (v = 0; v < cthis->numV; v++) {
			/* 若v未被收录，且dist[v]更小 */
			if (collected[v] == 0 && dist[v] < minDist) {
				minDist = dist[v];
				minV = v;
			}
		}
		/* 已经全部收录 */
		if (minDist == LOOC_GRAPH_NO_EDGE) {
			break;
		}
		/* 收录minV */
		collected[minV] = 1;
		/* 更新dist */
		for (v = 0; v < cthis->numV; v++) {
			/* 若v是minV的邻接点并且未被收录 */
			if (*(cthis->G + minV * cthis->_maxVertex + v) < LOOC_GRAPH_NO_EDGE
					&& collected[v] == 0) {
				if (*(cthis->G + minV * cthis->_maxVertex + v) < 0) {
					return looc_false; //若有负边,返回错误标记
				}
				/* 若收录minV使得dist[v]变小 */
				if (dist[minV] + *(cthis->G + minV * cthis->_maxVertex + v)
						< dist[v]) {
					dist[v] = dist[minV]
							+ *(cthis->G + minV * cthis->_maxVertex + v); //更新dist[v]
					path[v] = minV; //v的前驱点就是minV
				}
			}
		}
	}
	/* 释放collected的内存空间 */
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
static looc_bool loocAdjacencyGraph_Floyd(loocAdjacencyGraph* cthis,
		int D[][LOOC_DEFAULT_LOOCADJACENCYGRAPH_VERTEX],
		int path[][LOOC_DEFAULT_LOOCADJACENCYGRAPH_VERTEX]) {
	int i, j, k;
	/* 初始化 */
	for (i = 0; i < cthis->_maxVertex; i++) {
		for (j = 0; j < cthis->_maxVertex; j++) {
			D[i][j] = *(cthis->G + i * cthis->_maxVertex + j);
			path[i][j] = -1;
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
static int loocAdjacencyGraph_Prim(loocAdjacencyGraph* cthis,
		loocAdjacencyGraph* MST) {
	int* dist = looc_malloc(sizeof(int) * cthis->_maxVertex,
			"loocAdjacencyGraph_dist", looc_file_line);
	int* parent = looc_malloc(sizeof(int) * cthis->_maxVertex,
			"loocAdjacencyGraph_parent", looc_file_line);
	int i;
	int minV, minDist;
	int VCount;
	int totalWeight;
	/* 初始化，默认初始点下标是0 */
	for (i = 0; i < cthis->numV; i++) {
		dist[i] = *(cthis->G + 0 * cthis->_maxVertex + i);
		parent[i] = 0;	//暂且定义所有顶点的父节点都是初始点0
		MST->addVertex(MST, cthis->data_pool + i * cthis->_elementSize);
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
		for (i = 0; i < cthis->numV; i++) {
			/* 若i是minV的邻接点且未被收录 */
			if (dist[i] != 0 && loocAdjacencyGraph_existEdge(cthis, minV, i)) {
				if (*(cthis->G + minV * cthis->_maxVertex + i) < dist[i]) {
					dist[i] = *(cthis->G + minV * cthis->_maxVertex + i);
					parent[i] = minV;
				}
			}
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
static int loocAdjacencyGraph_Kruskal(loocAdjacencyGraph* cthis,
		loocAdjacencyGraph* MST) {
	int totalWeight = 0;
	int ECount = 0;	//收录的边数
	int i, j;
	int root1, root2;
	Edge e;
	/* 最小堆，保存每一条边 */
	loocHeap* heap = loocHeap_new(looc_file_line);
	heap->init(heap, LOOC_MIN_HEAP, cthis->numE, sizeof(Edge),
			Heap_compareStrategy);
	/* 并查集，保存顶点下标 */
	loocDisjointSet* set = loocDisjointSet_new(looc_file_line);
	set->init(set, cthis->numV, sizeof(int));
	/* 初始化 */
	for (i = 0; i < cthis->numV; i++) {
		for (j = 0; j < cthis->numV; j++) {
			/* 避免无向图中重复录入边 */
			if (loocAdjacencyGraph_existEdge(cthis, i, j)
					&& ((cthis->check == 1) || ((cthis->check == 0) && (i < j)))) {
				e.v1 = i;
				e.v2 = j;
				e.weight = cthis->getValueOfEdge(cthis, i, j);
				/* 插入边集 */
				heap->insert(heap, (void*) &e);
			}
		}
		/* MST包含所有顶点 */
		MST->addVertex(MST, cthis->data_pool + i * cthis->_elementSize);
		/* 插入顶点集合 */
		set->insert(set, (void*) &i);
	}
	while (ECount < cthis->numV - 1) {
		/* 取出权值最小的边 */
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
static void loocAdjacencyGraph_finalize(loocObject* object) {
	loocAdjacencyGraph* graph = SUB_PTR(object, loocObject, loocAdjacencyGraph);
	if (graph->data_pool) {
		looc_free(graph->data_pool);
	}
	if (graph->G) {
		looc_free(graph->G);
	}
	graph->numE = 0;
	graph->numV = 0;
}

/**
 * loocAdjacencyGraph的构造函数
 */
CTOR(loocAdjacencyGraph)
/* 调用父类的构造函数 */
	SUPER_CTOR(loocObject);
	cthis->_elementSize = 1;
	cthis->check = 0;
	cthis->data_pool = NULL;
	cthis->G = NULL;
	cthis->numE = 0;
	cthis->numV = 0;
	cthis->_maxVertex = LOOC_DEFAULT_LOOCADJACENCYGRAPH_VERTEX;
	/* 成员函数绑定 */
	FUNCTION_SETTING(init, loocAdjacencyGraph_init);
	FUNCTION_SETTING(insertEdge, loocAdjacencyGraph_insertEdge);
	FUNCTION_SETTING(deleteEdge, loocAdjacencyGraph_deleteEdge);
	FUNCTION_SETTING(addVertex, loocAdjacencyGraph_addVertex);
	FUNCTION_SETTING(existEdge, loocAdjacencyGraph_existEdge);
	FUNCTION_SETTING(getValueOfEdge, loocAdjacencyGraph_getValueOfEdge);
	FUNCTION_SETTING(outDegree, loocAdjacencyGraph_outDegree);
	FUNCTION_SETTING(inDegree, loocAdjacencyGraph_inDegree);
	FUNCTION_SETTING(DFS, loocAdjacencyGraph_DFS);
	FUNCTION_SETTING(BFS, loocAdjacencyGraph_BFS);
	FUNCTION_SETTING(topologySort, loocAdjacencyGraph_topologySort);
	FUNCTION_SETTING(Dijkstra, loocAdjacencyGraph_Dijkstra);
	FUNCTION_SETTING(Floyd, loocAdjacencyGraph_Floyd);
	FUNCTION_SETTING(Prim, loocAdjacencyGraph_Prim);
	FUNCTION_SETTING(Kruskal, loocAdjacencyGraph_Kruskal);
	FUNCTION_SETTING(loocObject.finalize, loocAdjacencyGraph_finalize);END_CTOR

/**
 * loocAdjacencyGraph的析构函数
 */
DTOR(loocAdjacencyGraph)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
