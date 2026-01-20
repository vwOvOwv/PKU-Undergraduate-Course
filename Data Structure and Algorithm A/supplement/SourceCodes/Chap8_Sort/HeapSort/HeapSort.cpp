
#include <iostream>
#include <stdlib.h>
using namespace std;
#include "../sort.h"
#include "MaxHeap.h"


//堆排序，Array[]为待排序数组，n为数组长度
template <class Record>
void sort(Record Array[], int n) 
{ 
	MaxHeap<Record> max_heap = MaxHeap<Record>(Array,n,n);	//建堆
	for (int i=0; i<n; i++)		 // 依次找出剩余记录中的最大记录，即堆顶
		max_heap.MoveMax();	
}


#include "../SortMain.h"
