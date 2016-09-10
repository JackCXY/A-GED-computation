#pragma once
#ifndef _VERIFY_GRAPH_H
#define _VERIFY_GRAPH_H
#include "graph.h"

typedef struct verifyGraphNode
{
	uchar verifyGraphNodeID;
	uchar verifyGraphNodeStr;
	verifyGraphNode(){}
	verifyGraphNode(const verifyGraphNode &t)
	{
		*this = t;
	}
	verifyGraphNode & operator= (const verifyGraphNode &t)
	{
		if (this != &t)
		{
			this->verifyGraphNodeID = t.verifyGraphNodeID;
			this->verifyGraphNodeStr = t.verifyGraphNodeStr;
			return *this;
		}
	}
};
class verifyGraph
{
public:
	uchar **adjMatrix;
	verifyGraphNode* graphNode;
	bool *flag;

	uchar gs; 
	int v;
	
public:
	verifyGraph(){ adjMatrix = NULL; }
	verifyGraph(const verifyGraph &g)
	{
		*this = g;
	}
	verifyGraph & operator= (const verifyGraph &g)
	{
		if (this != &g)
		{
			this->gs = g.gs;
			this->v = g.v;
			this->flag = NULL;
			this->graphNode = NULL;
			this->adjMatrix = NULL;

			if (g.flag) { this->flag = new bool[gs]; memcpy(this->flag, g.flag, sizeof(bool) * gs); }
			return *this;
		}

	}
	inline void initAdjMatrix(uchar **adj, const int &sz)
	{
		this->adjMatrix = new uchar*[sz];
		for (int i = 0; i < sz; i++)
		{
			this->adjMatrix[i] = new uchar[sz];
			memcpy(this->adjMatrix[i], adj[i], sizeof(uchar) * sz); //copy the data 
		}
	}


	inline verifyGraph(graph &ga)
	{
		this->gs = ga.v;
		this->v = ga.v;
		flag = new bool[gs]; memset(flag, 0, sizeof(bool) * gs);
		this->graphNode = new verifyGraphNode[gs];
		verifyGraphNode temp;

		for (int i = 0; i < gs; i++)
		{
			temp.verifyGraphNodeID = i;
			temp.verifyGraphNodeStr = ga.V[i];
			graphNode[i] = temp;
		}

		this->adjMatrix = new uchar *[gs];
		for (int i = 0; i < gs; i++)
		{
			this->adjMatrix[i] = new uchar[gs];
			memset(this->adjMatrix[i], 0xff, gs);
		}
		for (int i = 0; i < ga.v; i++)
		{
			vector<graph::edge> e = ga.E[i];
			for (int j = 0; j < e.size(); j++)
			{
				this->adjMatrix[i][e[j].to] = e[j].einfo;
			}
		}
	}
	inline void clear()
	{

		if (flag) delete[] flag; flag = NULL;
		if (graphNode) delete[] graphNode; graphNode = NULL;
		if (adjMatrix)
		{
			for (int i = 0; i < this->gs; i++)
			{
				if (adjMatrix[i]) delete[] adjMatrix[i];
				adjMatrix[i] = NULL;
			}
			delete[] adjMatrix;
			adjMatrix = NULL;
		}
		this->v = 0;
	}
	~verifyGraph()
	{
		this->clear();
	}
	void remove(verifyGraphNode &node, verifyGraphNode *gn, int pos)
	{
		assert(pos >= 0 && pos < this->gs && this->flag && !flag[pos]);
		node = gn[pos];
		this->flag[pos] = true;
		v--;
	}

	void undealGraphSize(vector<int> &undeal)
	{
		if (!this->flag || v == 0) return;
		undeal.clear();
		for (int i = 0; i < gs; i++)
		{
			if (!flag[i])  // flag[i] = false;
			{
				undeal.push_back(i);
			}
		}
	}

	void undealGraphSize(vector<int> &vertexSet, verifyGraphNode *vn, int* ms, int &maxVertex)
	{
		memset(ms, 0, 127 * sizeof(int));
		maxVertex = 0;
		if (!this->flag || v == 0) return;
		for (int i = 0; i < gs; i++)
		{
			if (!flag[i])
			{
				int str = vn[i].verifyGraphNodeStr;
				vertexSet.push_back(vn[i].verifyGraphNodeID);
				if (maxVertex < str)
					maxVertex = str;
				ms[str]++;
			}
		}
	}
public:
	int vertexDegree(const int &idx)
	{
		int sum = 0;
		for (int i = 0; i < v; i++)
		{
			if (this->adjMatrix[i][idx] != 0xff)
				sum++;
		}
		return sum;
	}
	void graphDegree(int *gd)
	{
		for (int i = 0; i < v; i++)
			gd[i] = vertexDegree(i);
	}

};
#endif
