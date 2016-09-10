#ifndef  _TREE_VERIFY_GRAPH_Node_H
#define  _TREE_VERIFY_GRAPH_Node_H
#include "verifyGraph.h"

static bool FLAG = false;
static verifyGraph oG1;
static verifyGraph oG2;

static verifyGraphNode gn1[255];
static verifyGraphNode gn2[255];
static uchar a1[255][255];
static uchar a2[255][255];

static int ds1[32];
static int ds2[32];
static int me1[32];
static int me2[32];

static int tmpDegree[127];
static int mv1[127];
static int mv2[127];
static int tmpV1[127];
static int tmpV2[127];
static int degree[127];
static int degreeGraph1[127];
static int degreeGraph2[127];

class treeVerifyGraphNode
{
public:
	
	uchar *matching;
	uchar *inverseMatching;
	verifyGraph uG1, uG2;
	int deep; 
	int labelCost; 
	bool visited;

	treeVerifyGraphNode(){ labelCost = 0; deep = 0; matching = inverseMatching = NULL;visited = false; }
	treeVerifyGraphNode(verifyGraph &g1, verifyGraph &g2)
	{
		if (!FLAG)
		{
			oG1 = g1;
			oG2 = g2;
			for (int i = 0; i < g1.gs; i++)
			{
				memcpy(a1[i], g1.adjMatrix[i], g1.gs * sizeof(uchar));
			}
			for (int i = 0; i < g2.gs; i++)
			{
				memcpy(a2[i], g2.adjMatrix[i], g2.gs * sizeof(uchar));
			}
			memcpy(gn1, g1.graphNode, sizeof(verifyGraphNode) * oG1.gs);
			memcpy(gn2, g2.graphNode, sizeof(verifyGraphNode) * oG2.gs);
			g1.graphDegree(degreeGraph1);
			g2.graphDegree(degreeGraph2);
			FLAG = !FLAG;
		}
		uG1 = g1;
		uG2 = g2;
		int gSize1 = uG1.gs;
		int gSize2 = uG2.gs;

		matching = new uchar[gSize1]; memset(matching, 255, gSize1 * sizeof(uchar));
		inverseMatching = new uchar[gSize2]; memset(inverseMatching, 255, gSize2 * sizeof(uchar));
		deep = 0;
		labelCost = 0;
		visited = false;
	}
	treeVerifyGraphNode & operator= (const treeVerifyGraphNode &tn) //deep copy
	{
		if(this != &tn)
		{
			uG1 = tn.uG1; 
			uG2 = tn.uG2;
			labelCost = tn.labelCost; 
			deep = tn.deep;
			int len1 = uG1.gs;
			int len2 =  uG2.gs;

			assert(tn.matching);
			matching = new uchar[len1]; memcpy(matching, tn.matching, sizeof(uchar) * len1);
			assert(tn.inverseMatching);
			inverseMatching = new uchar[len2]; memcpy(inverseMatching, tn.inverseMatching, sizeof(uchar) * len2);
			this->visited = false;
			return *this;
		}
	}	
	treeVerifyGraphNode(const treeVerifyGraphNode &tn) //deep copy
	{
		*this = tn;
	}
	~treeVerifyGraphNode()
	{
		if (matching) delete[]matching; matching = NULL;
		if (inverseMatching) delete[] inverseMatching; inverseMatching = NULL;
	}
public:
	inline void  addCost(int c)
	{
		this->deep += c;
	}
	bool operator< (const treeVerifyGraphNode &temp) const //
	{
		return (this->deep + this->labelCost) > (temp.deep + temp.labelCost);
	}
	bool allverifyGraphNodesUsed()
	{
		if (uG1.v == 0 && uG2.v == 0)
			return true;
		return false;
	}
	void labelVertexSet(verifyGraph &ug, verifyGraphNode *vn, vector<int> &vertexSet, int* &ms, int &maxVertex)
	{
		vertexSet.clear();
		memset(ms, 0, 127 * sizeof(int));
		maxVertex = 0;
		if (!ug.flag || ug.v == 0)  return;
		for (int i = 0; i < ug.gs; i++)
		{
			if (!ug.flag[i]) //ug.flag[i] = false;
			{
				int str = vn[i].verifyGraphNodeStr;
				vertexSet.push_back(vn[i].verifyGraphNodeID);
				if (maxVertex < str)
					maxVertex = str;
				ms[str]++;
			}
		}
	}
	inline void labelverifyGraphEdgeSet(vector<int> &vertexSet, int position, int &le, uchar  (*adj)[255], \
		int *ds, int* totalDegree, vector<int> &od, int &size, int* ms, int &maxEdge)
	{
		memset(tmpDegree, 0, 64 * sizeof(int));
		memset(ms, 0, 32 * sizeof(int));
		memset(degree, 0, 127 * sizeof(int));

		le = 0;
		maxEdge = 0;
		int count = 0;
		int lv = vertexSet.size();
		od.resize(lv, 0);

		size = 0;
		for (int i = 0; i < lv; i++)
		{
			if (i == position) continue;
			for (int j = i + 1; j < lv; j++)
			{
				if (j == position) continue;
				int from = vertexSet[i];
				int to = vertexSet[j]; //ensure that: to > from
				if (adj[from][to] != 255) //
				{
					int verifyGraphEdgeStr = (int)adj[from][to];
					if (maxEdge < verifyGraphEdgeStr)
						maxEdge = verifyGraphEdgeStr;
					ms[verifyGraphEdgeStr]++;
					le++;
					degree[i] ++;
					degree[j] ++;
				}//if-1.1
			}//for-1.1
		}//for-1

		for (int i = 0; i < lv; i++)
		{
			int from = vertexSet[i];
			if (i == position) continue;
			od[i] = totalDegree[from] - degree[i];
		}

		for (int i = 0; i < lv; i++)
		{
			if (count < degree[i]) count = degree[i];
			tmpDegree[degree[i]]++;
		}
		for (int i = count; i >= 0; i--)
		{
			int len = tmpDegree[i]; //chongdu 
			for (int l = 0; l < len; l++)
				ds[size++] = i;
		}
	}
public:
	inline int getNumberOfAdjacentverifyGraphEdges(uchar * &m, uchar(*a)[255], int i, const int &v)
	{
		int e = 0;
		for (int j = 0; j < v; j++)
		{
			if (j == i) continue;
			if (m[j] != 255 && a[i][j] != 255) // have been mapping
			{
				e += 1;
			}
		}
		return e;
	}
	inline void processverifyGraphEdges(treeVerifyGraphNode *tn, int &startIndex, int &endIndex, const int &v)
	{
		for (int e = 0; e < v; e++)
		{
			if (e == startIndex) continue;
			if (a1[startIndex][e] != 255)  // if-1 there is an verifyGraphEdge between start and start2 
			{
				int start2Index = e;
				if (tn->matching[start2Index] != 255) //if-1.1
				{
					int end2Index = tn->matching[start2Index];
					if (end2Index == 254 || end2Index == 253)//if- 1.1.1
						tn->addCost(1);
					else
					{
						if (a2[endIndex][end2Index] != 255)//if-1.1.1.1
						{
							int verifyGraphEdge = a1[startIndex][e];
							int verifyGraphEdge2 = a2[endIndex][end2Index];
							if (verifyGraphEdge != verifyGraphEdge2)
								tn->addCost(1); //verifyGraphEdge subtition
							else
								tn->addCost(0);
						}
						else
						{
							tn->addCost(1); //insert a verifyGraphEdge between endIndex and end2Index
						}//if-1.1.1.1
					}//
				}
			}
			else
			{
				int start2Index = e;
				if (tn->matching[start2Index] != 255)//if-1
				{ // other "end" has been handled
					int end2Index = tn->matching[start2Index];
					if (end2Index != 255 && end2Index != 254 && end2Index != 253) //if-1.1
					{
						if (a2[endIndex][end2Index] != 255)//if-1.1.1
						{
							tn->addCost(1); //delete the verifyGraphEdge between end2Index and endIndex
						}
					}//if-1.1
				}//if-1
			}//if-1
		}//for
	}
	inline vector<treeVerifyGraphNode *> generateSuccessors(const int &bound)
	{
		bool flag = false;
		vector<treeVerifyGraphNode *> successors;
		vector<int> undealVertexSet1, undealVertexSet2;
		int maxv1 = 0, maxv2 = 0;

		this->uG1.undealGraphSize(undealVertexSet1, gn1, mv1, maxv1);
		this->uG2.undealGraphSize(undealVertexSet2, gn2, mv2, maxv2);

		int n1 = undealVertexSet1.size();
		int n2 = undealVertexSet2.size();
		int sz1 = oG1.gs;
		int sz2 = oG2.gs;

		if (this->uG2.v == 0)
		{
			treeVerifyGraphNode *tn = new treeVerifyGraphNode(*this);
			int e = 0;
			for (int j = 0; j < n1; j++)
			{
				int i = undealVertexSet1[j];
				e += this->getNumberOfAdjacentverifyGraphEdges(tn->matching, a1, i, sz1); // get the verifyGraphEdge adjacent to the verifyGraphNode 
				tn->matching[i] = 254; // -2 = deletion

			}
			tn->addCost(n1);//the left verifyGraphNodes in u1
			tn->addCost(e);//the left verifyGraphEdges in u1
			if (tn->deep <= bound) // finding the mapping
			{
				tn->uG1.clear();
				successors.push_back(tn);
			}
			else
			{
				if (tn) delete tn; tn = NULL;
			}
		}
		else if (this->uG1.v == 0)
		{
			treeVerifyGraphNode *tn = new treeVerifyGraphNode(*this);
			int e = 0;
			for (int j = 0; j < n2; j++)
			{
				int i = undealVertexSet2[j];
				e += getNumberOfAdjacentverifyGraphEdges(tn->inverseMatching, a2, i, sz2);//t the verifyGraphEdge adjacent to the verifyGraphNode
				tn->inverseMatching[i] = 253; // -2 = insertion
			}
			tn->addCost(n2);
			tn->addCost(e);
			if (tn->deep <= bound)
			{
				tn->uG2.clear();
				successors.push_back(tn);
			}
			else
			{
				if (tn) delete tn; tn = NULL;
			}
		}
		else
		{
			int rankj = 0;
			for (int i = 0; i < n2; i++) //the order of A star
			{
				treeVerifyGraphNode *tn = new treeVerifyGraphNode(*this);
				verifyGraphNode start, end;

				int ranki = i;
				tn->uG1.remove(start, gn1, undealVertexSet1[rankj]);
				tn->uG2.remove(end, gn2, undealVertexSet2[ranki]);

				if (start.verifyGraphNodeStr != end.verifyGraphNodeStr) // the verifyGraphNode subtitution
					tn->addCost(1); //
				else
					tn->addCost(0);

				int startIndex = start.verifyGraphNodeID;
				int endIndex = end.verifyGraphNodeID;

				memcpy(tmpV1, mv1, sizeof(int) * 127);
				memcpy(tmpV2, mv2, sizeof(int) * 127);
				tmpV1[start.verifyGraphNodeStr]--;
				tmpV2[end.verifyGraphNodeStr]--;

				tn->matching[startIndex] = endIndex;
				tn->inverseMatching[endIndex] = startIndex;

				this->processverifyGraphEdges(tn, startIndex, endIndex, sz1);
				this->labelEditDistance(tn, undealVertexSet1, rankj, undealVertexSet2, ranki, tmpV1, tmpV2, maxv1, maxv2, bound, flag);
				if (flag)
					successors.push_back(tn);
				else
				{
					if (tn) delete tn; tn = NULL;
				}
			}
#if 0
			treeVerifyGraphNode *tn = new treeVerifyGraphNode(*this);
			verifyGraphNode deleted;
			tn->uG1.remove(deleted, gn1, undealVertexSet1[rankj]); //default 
			int i = deleted.verifyGraphNodeID;
			tn->matching[i] = 254; // deletion
			tn->addCost(1);
			//
			memcpy(tmpV1, mv1, sizeof(int) * 127);
			tmpV1[deleted.verifyGraphNodeStr]--;

			int e = getNumberOfAdjacentverifyGraphEdges(tn->matching, a1, i, sz1);
			tn->addCost(e);
			this->labelEditDistance(tn, undealVertexSet1, rankj, undealVertexSet2, -1, tmpV1, mv2, maxv1, maxv2, bound, flag);

			if (flag)
				successors.push_back(tn);
			else
			{
				if (tn) delete tn; tn = NULL;
			}
#endif
		}
		return successors;
	}
	void treeVerifyGraphNode::labelEditDistance(treeVerifyGraphNode *tn, vector<int> &uv1, int rankj, vector<int> &uv2, \
		int ranki, int *tmpV1, int *tmpv2, int &maxv1, int &maxv2, const int &bound, bool &flag)
	{
		int d = tn->deep;
		int sv = 0;
		int maxv = min(maxv1, maxv2);

		for (int i = 0; i <= maxv; i++)  //label distance 
		{
			if (tmpV1[i] && tmpv2[i])
				sv += min(tmpV1[i], tmpv2[i]);
		}

		d += max(tn->uG1.v, tn->uG2.v) - sv;
		if (d > bound)
		{
			flag = false;
			return;
		}

		int le1 = 0, le2 = 0;
		int size1 = 0, size2 = 0;
		vector<int> dg1, dg2;
		vector<int> od1, od2;

		int maxe1 = 0, maxe2 = 0;
		//labelverifyGraphEdgeSet(uv1, rankj, le1, a1, dg1, degreeGraph1, od1, me1, maxe1);
		//labelverifyGraphEdgeSet(uv2, ranki, le2, a2, dg2, degreeGraph2, od2, me2, maxe2);

		labelverifyGraphEdgeSet(uv1, rankj, le1, a1, ds1, degreeGraph1, od1, size1, me1, maxe1); //od1
		labelverifyGraphEdgeSet(uv2, ranki, le2, a2, ds2, degreeGraph2, od2, size2, me2, maxe2); //od2

		int se = 0;
		int maxe = min(maxe1, maxe2);
		for (int i = 0; i <= maxe; i++)
		{
			if (me1[i] && me2[i])
				se += min(me1[i], me2[i]);
		}
		d += max(le1, le2) - se;
		if (d > bound)
		{
			flag = false;
			return;
		}
		//abs (od1, od2)
		//int dd = common::degreeEditDistance(dg1, dg2);
		int dd = common::degreeEditDistance(ds1, size1, ds2, size2);
		dd += common::outDegree(od1, od2);
		d += (max(le1, le2) - se) < dd ? (dd - max(le1, le2) + se) : 0;
		if (d > bound)
		{
			flag = false;
			return;
		}
		else
		{
			tn->labelCost = d - tn->deep;
			flag = true;
			return;
		}
		//branch Distance 
	}

#if 0
	void labelverifyGraphEdgeSet(vector<int> &vertexSet, int position, int &le, int(*adj)[255], vector<int> &degree, int* totalDegree, vector<int> &od, int* ms, int &maxEdge)
	{
		memset(ms, 0, 127 * sizeof(int));
		le = 0;
		maxEdge = 0;
		int lv = vertexSet.size();
		degree.resize(lv, 0);
		od.resize(lv, 0);

		for (int i = 0; i < lv; i++)
		{
			if (i == position) continue;
			for (int j = i + 1; j < lv; j++)
			{
				if (j == position) continue;
				int from = vertexSet[i];
				int to = vertexSet[j]; //ensure that: to > from
				if (adj[from][to] != 255) //
				{
					int verifyGraphEdgeStr = (int)adj[from][to];
					if (maxEdge < verifyGraphEdgeStr)
						maxEdge = verifyGraphEdgeStr;
					ms[verifyGraphEdgeStr]++;
					le++;
					degree[i] ++;
					degree[j] ++;
				}//if-1.1
			}//for-1.1
		}//for-1
		for (int i = 0; i < lv; i++)
		{
			int from = vertexSet[i];
			if (i == position) continue;
			od[i] = totalDegree[from] - degree[i];
		}

	}
	void branchQgramVertex(verifyGraphNode *vn, vector<int> &vertexSet, int position, int **&adj, int &j, string &bqv)
	{

		char temp[32];
		temp[0] = vn[j].verifyGraphNodeStr;
		int count = 1;
		int lv = vertexSet.size();
		int from = vertexSet[j];

		for (int i = 0; i < lv; i++)
		{
			if (i == position) continue;
			int to = vertexSet[i];
			if (adj[from][to] != 255)
			{
				temp[count++] = adj[from][to];
			}
		}
		temp[count++]  = '\0';
		sort(temp + 1, temp + count);
		bqv = temp;
	}
	void branchQgramSet(verifyGraphNode *vn, vector<int> &vertexSet, int position, int **&adj, vector<string> &bqs)
	{
		bqs.clear();
		int lv = vertexSet.size();
		for(int i = 0; i < lv;i++)
		{
			if(i == position) continue;
			string bqv;
			branchQgramVertex(vn, vertexSet, position, adj, i, bqv);
			bqs.push_back(bqv);
		}
	}
	int unionBranchQgram(vector<string> &bqs1, vector<string> &bqs2)
	{
		sort(bqs1.begin(), bqs1.end());
		sort(bqs2.begin(), bqs2.end());
		int sum = 0;
		int sz1 = bqs1.size(), sz2 = bqs2.size();
		int i = 0, j = 0;
		while(i < sz1 && j < sz2)
		{
			if(bqs1[i] == bqs2[j])
			{
				sum ++;
				i++;
				j++;
			}
			else if(bqs1[i] < bqs2[j]) i++;
			else
				j++;
		}
		return sum;
	}
#endif

};
#endif

