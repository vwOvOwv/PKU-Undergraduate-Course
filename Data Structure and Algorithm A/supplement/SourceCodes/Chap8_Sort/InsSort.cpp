#include <iostream>
#include <stdlib.h>
using namespace std;
#include "sort.h"

template <class Record>
void InsertSort (Record Array[], int n)  {
	// 直接插入排序，Array[]为待排序数组，n为数组长度
	Record TempRecord;						// 临时变量
	for (int i=1; i<n; i++)  {					// 依次插入第i个记录
		TempRecord=Array[i];					// 先把记录Array[i]保留在临时记录变量中
		int j = i-1;							// 内存循环变量从j从i-1开始
		// 往前寻找记录i的正确位置
		while (j >= 0 && TempRecord < Array[j])  {
			Array[j+1] = Array[j];				// 将那些大于等于记录i的记录后移
			j = j - 1;						// 下标j前移
	    }
	    Array[j+1] = TempRecord; 				// 此时j+1就是记录i的正确位置，回填
	}
}

template <class Record>
void sort(Record* Array, int n) {
	InsertSort(Array, n);
}

#include "SortMain.h"
