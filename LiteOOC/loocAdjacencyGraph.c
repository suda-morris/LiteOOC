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
#include <string.h>

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
			*(cthis->G + v * maxVertex + w) = LOOC_GRAPH_NO_EDGE;
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
			|| ((*(cthis->G + v1 * cthis->_maxVertex + v2))
					== LOOC_GRAPH_NO_EDGE)) {
		return looc_false;
	} else {
		return looc_true;
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
			if ((*(cthis->G + v * cthis->_maxVertex + i)) != LOOC_GRAPH_NO_EDGE) {
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
			if ((*(cthis->G + i * cthis->_maxVertex + v)) != LOOC_GRAPH_NO_EDGE) {
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
		if ((*(cthis->G + v * cthis->_maxVertex + i)) != LOOC_GRAPH_NO_EDGE) {
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
				if ((*(cthis->G + i * cthis->_maxVertex + j))
						!= LOOC_GRAPH_NO_EDGE) {
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
				if ((*(cthis->G + v * cthis->_maxVertex + i))
						!= LOOC_GRAPH_NO_EDGE) {
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
	FUNCTION_SETTING(outDegree, loocAdjacencyGraph_outDegree);
	FUNCTION_SETTING(inDegree, loocAdjacencyGraph_inDegree);
	FUNCTION_SETTING(DFS, loocAdjacencyGraph_DFS);
	FUNCTION_SETTING(BFS, loocAdjacencyGraph_BFS);
	FUNCTION_SETTING(topologySort, loocAdjacencyGraph_topologySort);
	FUNCTION_SETTING(loocObject.finalize, loocAdjacencyGraph_finalize);END_CTOR

/**
 * loocAdjacencyGraph的析构函数
 */
DTOR(loocAdjacencyGraph)
/* 调用父类的析构函数，实质上就是子类实现的finalize方法 */
	SUPER_DTOR(loocObject);END_DTOR
