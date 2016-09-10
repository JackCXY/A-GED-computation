#ifndef _EDIT_DISTANCE_H
#define _EDIT_DISTANCE_H
#include "treeVerifyGraphNode.h"
#include "qSort.h"

class editDistance
{
public:
	stack<treeVerifyGraphNode *> s;
public:
	editDistance(){  }
	//optimize the serach processing 
	int getEditDistance(graph &ga, graph &gb, int bound)
	{
		verifyGraph g1(ga);
		verifyGraph g2(gb);

		vector<treeVerifyGraphNode *> successors;
		treeVerifyGraphNode *start = new treeVerifyGraphNode(g1, g2);
		
		treeVerifyGraphNode *Pmin = NULL;
		s.push(start);
		const int T = bound + 1;
		int ged = bound + 1;

		while(!s.empty())
		{
			Pmin = s.top();
			if(Pmin->visited)
			{
				if (Pmin) delete Pmin; s.pop(); 
			}                                    
			else
			{
				Pmin->visited = true;
				if (Pmin->allverifyGraphNodesUsed()) //leaf Node[pruning with(LB, UB)]
				{
					int temp = Pmin->deep;
					if (temp < ged) //重复路径选择(1.所有的最优路径都等价于A=x) 
					{
						ged = temp;
						bound = temp;
						bound = bound - 1;
					}
					cout << ged << endl;
					if (Pmin) delete Pmin; s.pop();
					continue;
				}
				successors = Pmin->generateSuccessors(bound); 
				int size = successors.size();
				if (size > 0)
				{
					random_quikSort(&successors[0], 0, size - 1);
					for (int i = successors.size() - 1; i >= 0; i--)
					{
						s.push(successors[i]);
					}

				}
			}
		}
		if (ged == T) return -1;
		return ged;
	}
	
	~editDistance()
	{
		FLAG = false;
	}
	
};
#endif
