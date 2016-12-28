/*
 ============================================================================
 Name        : loocLinkedGraph.h
 Author      : morris
 Version     :
 Copyright   : Your copyright notice
 Description : 邻接表存储的图
 ============================================================================
 */

#ifndef LITEOOC_LOOCLINKEDGRAPH_H_
#define LITEOOC_LOOCLINKEDGRAPH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <looc.h>
#include "loocObject.h"
#include "loocSingleList.h"

	/* 默认顶点的最大数*/
#define LOOC_DEFAULT_LOOCLINKEDGRAPH_VERTEX			(20)

	/**
	 * 邻接表存储的图
	 */
	CLASS(loocLinkedGraph) {
		/* 继承自loocObject类  */
		EXTENDS(loocObject);
		/* 是否是有向图，1为有向图，0为无向图 */
		int check;
		/* 图顶点中每个元素的大小 */
		int _elementSize;
		/* 顶点数的上限 */
		int _maxVertex;
		/* 顶点数 */
		int numV;
		/* 边数 */
		int numE;
		/* 存储顶点的数组 */
		loocSingleList** h;
		/* 顶点数据 */
		void* data_pool;
		/* 初始化图结构 */
		void (*init)(loocLinkedGraph* cthis, int maxVertex, int elementSize,
				int icheck);
		/* 插入顶点操作 */
		looc_bool (*addVertex)(loocLinkedGraph* cthis, void* data);
		/* 插入边操作 */
		looc_bool (*insertEdge)(loocLinkedGraph* cthis, int v1, int v2,
				int weight);
		/* 判定顶点之间是否有边存在 */
		looc_bool (*existEdge)(loocLinkedGraph* cthis, int v1, int v2);
		/* 计算某个节点的出度 */
		int (*outDegree)(loocLinkedGraph* cthis, int v);
	};

#ifdef __cplusplus
}
#endif
#endif /* LITEOOC_LOOCLINKEDGRAPH_H_ */
