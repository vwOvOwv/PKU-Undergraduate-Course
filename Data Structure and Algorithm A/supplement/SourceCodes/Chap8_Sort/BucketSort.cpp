#include <iostream>
#include <stdlib.h>
using namespace std;
#include "sort.h"

template <class Record>
void BucketSort(Record Array[], int n, int max) {	
    // 桶式排序，Array[]为待排序数组，数组长度为n，所有记录都位于区间[0,max)上
    int* TempArray = new Record[n];		// 临时数组
    int* count = new int[max];			// 小于或等于i的元素个数
    int i;

    for (i=0; i<n; i++)					// 把序列复制到临时数组
  	    TempArray[i] = Array[i];
    for (i=0; i<max; i++)				// 所有计数器初始都为0
  	    count[i] = 0;
    for (i=0; i<n; i++)					// 统计每个取值出现的次数
	    count[Array[i]]++;
    for (i=1; i<max; i++)				// 统计小于等于i的元素个数
  	    count[i] = count[i-1]+count [i];
    for (i=n-1;i>=0;i--)				// 从尾部开始按顺序输出有序序列，保证排序的稳定性
        Array[--count[TempArray[i]]] = TempArray[i];
}


template <class Record>
void sort(Record* Array, int n) {
  BucketSort(Array, n, 80); 
}

#include "SortMain.h"
