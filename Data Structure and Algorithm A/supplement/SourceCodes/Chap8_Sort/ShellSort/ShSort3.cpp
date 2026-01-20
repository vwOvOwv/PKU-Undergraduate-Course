#include <iostream>
#include <stdlib.h>
using namespace std;
#include "../sort.h"

/*
template <class Record>
void ModInsSort(Record Array[], int n, int delta) {	// 修改的插入排序算法，参数delta表示当前的增量
	for (int i=delta; i<n; i+=delta)			// 对子序列中第i个记录，寻找合适的插入位置
		for (int j=i; j>=delta; j-=delta)  {		//  j以dealta为步长向前寻找逆序对进行调整
			if (Array[j] < Array[j-delta])
				swap(Array, j, j-delta);
			else break;
		}
}
*/

template <class Record>
void ModInsSort(Record Array[], int n, int delta) {	
	// 针对变化的增量而修改的插入排序算法，参数delta表示当前的增量
	Record TempRecord;						// 临时变量
	for (int i=delta; i<n; i+=delta) {		// 对子序列中第i个记录排序
		TempRecord=Array[i];
		int j = i-delta;
		//从i开始往前寻找记录i的正确位置
		while (j>=0 && TempRecord < Array[j]) {
			Array[j+delta] = Array[j];		//将那些大于等于记录i的记录后移
			j = j - delta;
	    }
	    Array[j+delta] = TempRecord; 			//此时j后面就是记录i的正确位置，回填
	}
}

template <class Record>
void ShellSort(Record Array[], int n)  {	 		// Shell排序，Array[]为待排序数组，n为数组长度 
	for (int delta=n/3; delta>=3; delta/=3)			//增量delta每次除以3递减
		for (int j=0; j<delta; j++)					//分别对delta个子序列进行插入排序
			ModInsSort(&Array[j],n-j, delta);
	ModInsSort(Array, n, 1);
}

template <class Record>
void sort(Record* Array, int n) {
	ShellSort(Array, n);
}

#include "../SortMain.h"
