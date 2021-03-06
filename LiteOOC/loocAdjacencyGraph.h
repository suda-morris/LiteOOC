/*
 ============================================================================
 Name        : loocAdjacencyGraph.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 邻接矩阵存储的图
 ============================================================================
 */

#ifndef LITEOOC_LOOCADJACENCYGRAPH_H_
#define LITEOOC_LOOCADJACENCYGRAPH_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <looc.h>
#include "loocObject.h"
#include <limits.h>

	/* 默认顶点的最大数*/
#define LOOC_DEFAULT_LOOCADJACENCYGRAPH_VERTEX			(10)

	/* ∞设为双字节无符号整数的最大值65535*/
#define LOOC_GRAPH_NO_EDGE 								(99999999)

	/**
	 * 加权图的邻接矩阵存储结构
	 */
	CLASS(loocAdjacencyGraph) {
		/* 继承自loocObject类  */
		EXTENDS(loocObject);
		/* 是否是有向图,1为有向图，0为无向图 */
		int check;
		/* 图顶点中每个元素的大小 */
		int _elementSize;
		/* 顶点数的上限 */
		int _maxVertex;
		/* 顶点数 */
		int numV;
		/* 边数 */
		int numE;
		/* 邻接矩阵，存放边的权重 */
		int** G;
		/* 顶点的数据 */
		void* data_pool;
		/* 图的初始化函数 */
		void (*init)(loocAdjacencyGraph* cthis, int maxVertex, int elementSize,
				int iCheck);
		/* 插入边操作 */
		looc_bool (*insertEdge)(loocAdjacencyGraph* cthis, int v1, int v2,
				int weight);
		/* 删除边 */
		looc_bool (*deleteEdge)(loocAdjacencyGraph* cthis, int v1, int v2);
		/* 插入顶点 */
		looc_bool (*addVertex)(loocAdjacencyGraph* cthis, void* data);
		/* 判定顶点之间是否有边存在 */
		looc_bool (*existEdge)(loocAdjacencyGraph* cthis, int v1, int v2);
		/* 获取边的权值 */
		int (*getValueOfEdge)(loocAdjacencyGraph* cthis, int v1, int v2);
		/* 计算某个顶点的出度 */
		int (*outDegree)(loocAdjacencyGraph* cthis, int v);
		/* 计算某个顶点的入度 */
		int (*inDegree)(loocAdjacencyGraph* cthis, int v);
		/* 深度优先遍历 */
		void (*DFS)(loocAdjacencyGraph* cthis, int v,
				void (*action)(void* node, void* args), void* args);
		/* 广度优先遍历 */
		void (*BFS)(loocAdjacencyGraph* cthis, int v,
				void (*action)(void* node, void* args), void* args);
		/* 拓扑排序 */
		looc_bool (*topologySort)(loocAdjacencyGraph* cthis, int order[]);
		/* 单源最短路径 */
		looc_bool (*Dijkstra)(loocAdjacencyGraph* cthis, int S, int dist[],
				int path[]);
		/* 多源最短路径(每对顶点间的最短路径) */
		looc_bool (*Floyd)(loocAdjacencyGraph* cthis,
				int D[][LOOC_DEFAULT_LOOCADJACENCYGRAPH_VERTEX],
				int path[][LOOC_DEFAULT_LOOCADJACENCYGRAPH_VERTEX]);
		/* 最小生成树Prim算法 ,用于稠密图，以顶点为基础*/
		int (*Prim)(loocAdjacencyGraph* cthis, loocAdjacencyGraph* MST);
		/* 最小生成树Kruskal算法 ,用于稀疏图，以边为基础*/
		int (*Kruskal)(loocAdjacencyGraph* cthis, loocAdjacencyGraph* MST);
	};

#ifdef __cplusplus
}
#endif
#endif /* LITEOOC_LOOCADJACENCYGRAPH_H_ */
