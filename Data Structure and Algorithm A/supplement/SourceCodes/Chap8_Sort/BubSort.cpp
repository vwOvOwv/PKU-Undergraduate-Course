#include <iostream>
#include <stdlib.h>
using namespace std;
#include "sort.h"

template <class Record>
void BubbleSort(Record Array[], int n)  {	// 优化的冒泡排序，Array[]为待排序数组，n为数组长度
	bool NoSwap;			// 是否发生了交换的标志
	for (int i=0; i<n-1; i++)  {
	NoSwap = true;		// 标志初始为真
	for (int j=n-1; j>=i; j--)
	  if (Array[j] < Array[j-1])  {	// 判断（Array[j-1]，Array[j]）是否逆置
		swap(Array, j, j-1);	// 交换逆置对Array[j]，Array[j-1]
		NoSwap = false;		// 发生了交换，标志变为假
	  }
	if (NoSwap)  			// 如果没发生过交换，表示已排好序，结束算法
		return;
	}
}

template <class Record>
void sort(Record* Array, int n) {
  BubbleSort(Array, n);
}

#include "SortMain.h"