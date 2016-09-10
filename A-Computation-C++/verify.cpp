#include "editDistance.h"
#include "graph.h"
#include "store.h"

int main(int argc, char *argv[])
{
	struct timeval start,end; 
	float timeuse; 
	if(argc < 5) { cout << "query queryfile d blob" << endl;exit(0);}

	const char *query = argv[1];
	const char *queryFile = argv[2];
	int d = atoi(argv[3]);

	ifstream fin(queryFile);
	int size;
	fin >> size;
	vector<vector<int > > verifyGraphSet;
	for(int i = 0; i < size;i++)
	{
		vector<int> tmp;
		int sz;
		fin >> sz;
		for(int j = 0; j < sz;j++)
		{
			int value;
			fin>> value;
			tmp.push_back(value);
		}
		sort(tmp.begin(), tmp.end());
		verifyGraphSet.push_back(tmp);
	}
	cout << "begin verify " << endl;

	vector<graph> queryGraph = graph::readGraphMemory(query, 50);	
	int graphNumber = queryGraph.size();
	cout << "begin verify graph edit distance" << endl;
	string path(argv[4]);
	store st(path + ".idx");
	gettimeofday(&start, NULL); 

	int real = 0;
	for (int j = 0; j < graphNumber; j++)
	{
		vector<int> result;
		if(access("./verify/", F_OK | R_OK) == -1)
		{ 
			if(mkdir("./verify", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) 
				cout << "mkdir error" << endl;
		}
		string vout = "./verify/" + common::inToString(j);	
		st.extractGraphBlob((path +".blb").c_str(), vout.c_str(), verifyGraphSet[j]);
		editDistance::verifyEditDistance(vout, verifyGraphSet[j].size(), queryGraph[j], d, result);
		real += result.size();
		if(j % 10 == 0) cout << j * graphNumber/100  << "%" << endl;
	}
	gettimeofday(&end, NULL); 
	timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec; 
	timeuse = timeuse * 1.0 / 1000000; 

	cout << "real size:" << real * 1.0 / graphNumber << ", " << timeuse << "s" << endl;
	return 0;
}
