#include "editDistance.h"
#include "graph.h"
#include "store.h"
int main(int argc, char *argv[])
{
	clock_t start, end;
	float timeuse;
	editDistance ed;
	start = clock();
	vector<graph> query = graph::readGraphMemory(AIDS_Query.c_str(), 50);
	graph g1 = query[9], g2 = query[10];
	int bound = g1.e + g2.e + max(g1.v, g2.v);
	//int bound = 15;
	if (g1.v < g2.v)
	{
		cout << ed.getEditDistance(g1, g2, bound) << endl;
	}
	else
	{
		cout << ed.getEditDistance(g2, g1, bound) << endl;
	}
	end = clock();
	cout << "time:" << (end - start) * 1.0 / CLOCKS_PER_SEC << endl;
	return 0;
}
