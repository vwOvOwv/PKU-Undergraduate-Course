#include <iostream>
#include "setm.h"

using namespace std;

struct WV {
		int weight;
		int value;
	};



#define N 20
WV wv[20] = {
		{52,7946		},
		{7160,387		},
		{7346,7505		},
		{816,577		},
		{961,6021		},
		{5262,8278		},
		{4163,931		},
		{1003,9738		},
		{7914,1683		},
		{320,475		},
		{1103,8280		},
		{6180,1198		},
		{1334,2791		},
		{5812,2608		},
		{6930,5515		},
		{4451,9602		},
		{3797,3160		},
		{2292,5940		},
		{1835,1807		},
		{773,4208		}
	};

/*
#define N 8
WV wv[8] = {
	{1,11},
	{11,21},
	{21,31},
	{23,33},
	{33,43},
	{43,53},
	{45,55},
	{55,65}
	};

    typedef mySet<8> Set;
    int MAX_WEIGHT = 110;
	int n = 8;
*/
    typedef mySet<20> Set;
	int MAX_WEIGHT = 20000;			// 对setsize =20 的数据集
	int n = 20;

	Set tempSet, solutionSet;
	int maxValue;
	int count=0;
	int leaf_cnt=0;

	void search(int i, int w, int v) {
		if (w <= MAX_WEIGHT && v > maxValue) {
			solutionSet = tempSet;
			maxValue = v;
		}
		count++;
		if (i >= n || w > MAX_WEIGHT) {
			leaf_cnt++;
			return;
		}
		search(i + 1, w, v);			// 物品i未放入背包
		tempSet.set(i);					// 缺省第2个参数为true
		search(i + 1, w + wv[i].weight, v + wv[i].value);
		tempSet.reset(i);			
		
	}
	void CalSolution() {
		tempSet.reset();
		solutionSet.reset();
		maxValue = 0;
		search(0, 0, 0);
	}

int main() {
	CalSolution();
	cout << solutionSet << endl;
	cout << "max value = " << maxValue << endl;
	cout << "count = " << count << " leaf_cnt = " << leaf_cnt << endl;
        return 0;
}

