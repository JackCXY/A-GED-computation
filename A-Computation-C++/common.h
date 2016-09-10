#ifndef _COMMON_H
#define _COMMON_H
#include "stdafx.h"

class common
{
public:

	static string inToString(string prefix, int v, int e)
	{
		stringstream ss;
		ss << v << e;
		return prefix + ss.str();
	}
	static string inToString(int n)
	{
		stringstream ss;
		ss << n;
		return ss.str();
	}
	static int degreeEditDistance(int *ds1, int &size1, int *ds2, int &size2)
	{
		
		int s1 = 0, s2 = 0;
		int size = min(size1, size2);

		for (int i = 0; i < size; i++)
		{
			if (ds1[i] < ds2[i])
				s1 += ds2[i] - ds1[i];
			else
				s2 += ds1[i] - ds2[i];
		}
		for(int i = size ; i < size1; i++)
			s2+= ds1[i];
		for(int i = size ; i < size2; i++)
			s1+= ds2[i];
		if (s1 % 2 == 0)
		{
			if (s2 % 2 == 0)
				return s1 / 2 + s2 / 2 ;
			else
				return s1 / 2 + s2 / 2 + 1 ;
		}
		else
		{
			if (s2 % 2 == 0)
				return s1 / 2 + s2 / 2 + 1;
			else
				return s1 / 2 + s2 / 2 + 2;
		}
	}
	static int outDegree(vector<int> od1, vector<int> od2)
	{
		sort(od1.begin(), od1.end(), greater<int> ());
		sort(od2.begin(), od2.end(), greater<int> ());
		int sz1 = od1.size(), sz2 = od2.size(); 
		int sz = min(sz1, sz2);
		int sum = 0;
		for (int i = 0; i < sz; i++)
		{
			if (od1[i] < od2[i]) sum += od2[i] - od1[i];
			else sum += od1[i] - od2[i];
		}
		if (sz == sz1)
		{
			for (int i = sz; i < sz2; i++)
				sum += od2[i];
		}
		else 
		{
			for (int i = sz; i < sz1; i++)
				sum += od1[i];
		}
		return sum;
	}
	static int degreeEditDistance(vector<int> ds1, vector<int> ds2)
	{
		int sz = max(ds1.size(), ds2.size());
		for (int i = ds1.size(); i < sz; i++)
			ds1.push_back(0);
		for (int i = ds2.size(); i < sz; i++)
			ds2.push_back(0);
		int s1 = 0, s2 = 0;
		int size = ds1.size();
		sort(ds1.begin(), ds1.end(), greater<int>());
		sort(ds2.begin(), ds2.end(), greater<int>());
	
		for (int i = 0; i < size; i++)
		{
			if (ds1[i] < ds2[i])
			{
				s1 += ds2[i] - ds1[i];
			}
			else
				s2 += ds1[i] - ds2[i];
		}
		if (s1 % 2 == 0)
		{
			if (s2 % 2 == 0)
				return s1 / 2 + s2 / 2 ;
			else
				return s1 / 2 + s2 / 2 + 1 ;
		}
		else
		{
			if (s2 % 2 == 0)
				return s1 / 2 + s2 / 2 + 1;
			else
				return s1 / 2 + s2 / 2 + 2;
		}
	}
	static int unionStringSet(map<string, int> &s1, map<string, int> &s2)
	{
		map<string, int> ::iterator iter, it;
		int sum = 0;
		for (iter = s1.begin(); iter != s1.end(); ++iter)
		{
			it = s2.find(iter->first);
			if (it != s2.end())
				sum += min(iter->second, it->second);
		}
		return sum;
	}

	//unionSet<>(Set A * B) for vertex and edge label
	static int unionSet(map<int, int> &lv1, map<int, int> &lv2)
	{
		map<int, int>::iterator iter, it;
		int sum = 0;
		for (iter = lv1.begin(); iter != lv1.end(); ++iter)
		{
			it = lv2.find(iter->first);
			if (it != lv2.end())
				sum += iter->second < it->second ? iter->second : it->second;
		}
		return sum;
	}
	//using for the invertIndex
	//the edit distance between two star structure
	//there may occur the problem that: s1[i] occupied two position
	static int starEditDistance(vector<int> &s1, vector<int> &s2)
	{
		int sameLabel = 0;
		int ed = 0;
		int len1 = s1.size(), len2 = s2.size();
		if (len1 == 0) return len2;
		if (len2 == 0) return len1;
		
		if (s1[len1 - 1] != s2[len2 - 1])
			ed += 1;
		int i = len1 - 2, j = len2 - 2;
		while (i >= 0 && j >= 0)
		{
			if (s1[i] == s2[j])
			{
				sameLabel++;
				i--;
				j--;
			}
			else if (s1[i] < s2[j])
				j--;
			else
				i--;
		}
		ed += max(len1 - 1, len2 - 1) - sameLabel;
		ed += s1.size() < s2.size() ? s2.size() - s1.size() : s1.size() - s2.size();
		return ed;
	}
	//using in the C-star
	static vector<vector<double >> simMatrix(vector<vector<int> > &s1, vector<vector<int >> &s2)
	{
		int r = s1.size();
		int c = s2.size();
		int max = r < c ? c : r;
		vector<double> vd(max, 0.0);
		vector<vector<double > > sim(max, vd);
		//two different init method 
		bool f = (r == max);
		if (f) // s1.size() > s2.size();
		{
			for (int i = 0; i < r; i++)
			{
				int j = 0;
				for (; j < c; j++)
					sim[i][j] = starEditDistance(s1[i], s2[j]);
				for (; j < r; j++)
					sim[i][j] = s1[i].size();
			}
		}
		else
		{
			int i = 0;
			for (; i < r; i++)
				for (int j = 0; j < c; j++)
					sim[i][j] = starEditDistance(s1[i], s2[j]);
			for (; i < c; i++)
				for (int j = 0; j < c; j++)
					sim[i][j] = s2[j].size();
		}
		return sim;
	}
	static vector<int> readFileVector(const char *file)
	{
		vector<int> fv;
		ifstream fin(file);
		int value;
		while (fin >> value)
		{
			fv.push_back(value);
		}
		fin.close();
		return fv;
	}
	//Jaccard distance 
	//we must assuming that: the vector has been ordered before into the process
	//Warning that: v1 and v2 must sort according to its value

	static int unionElem(vector<int> &v1, vector<int> &v2)
	{
		int i = 0, j = 0;
		int total = 0;
		while (i < v1.size() && j < v2.size())
		{
			if (v1[i] == v2[j])
			{
				total++;
				i++;
				j++;
			}
			else if (v1[i] < v2[j]) i++;
			else
				j++;

 		}
		return total;
	}
};

#endif
