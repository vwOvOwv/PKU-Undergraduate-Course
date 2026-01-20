#include <iostream>
#include <stdlib.h>

#include "sort.h"

template <class Record>
void SelectSort(Record Array[], int n)  {	// 直接选择排序，Array[]为待排序数组，n为数组长度
	for (int i=0; i<n-1; i++)  {			// 依次选出第i小的记录，即剩余记录中最小的那个
        int Smallest = i;				// 首先假设记录i就是最小的
        for (int j=i+1;j<n; j++)			// 开始向后扫描所有剩余记录
            if (Array[j] < Array[Smallest])
      	       Smallest = j;			// 如果发现更小的记录，记录它的位置
        swap(Array, i, Smallest);			// 交换Array[i]和 Array[Smallest]，第i小的记录到位
	}
}

template <class Record>
void sort(Record* Array, int n) {
  SelectSort(Array, n);
}

#include "SortMain.h"
