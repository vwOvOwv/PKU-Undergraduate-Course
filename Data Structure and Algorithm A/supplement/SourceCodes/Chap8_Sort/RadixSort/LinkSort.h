#pragma once
#include "../sort.h"

template <class Record>
void PrintAddr(Record *Array, int n) {
	cout << "数组内容:" << endl;
	for (int i=0; i<n; i++)  			// 打印数组内容
		cout << Array[i].key << "  " ;
	cout << endl;
	cout << "索引内容:" << endl;
	for (i=0; i<n; i++)  				// 打印数组索引内容
		cout << Array[i].next << "   " ;
	cout << endl;
}

//输出序列中所有内容
template <class Record>
void PrintArray(Record* Array, int first) 
{	//first为静态链Array中第一个记录的下标

	int tmp;							//用来扫描整个链的指针
	tmp = first;
	while (tmp != -1) 
	{
		cout << Array[tmp].key <<" ";	//输出记录
		tmp = Array[tmp].next;			//继续指向下一个记录
	}
	cout << '\n';
}

