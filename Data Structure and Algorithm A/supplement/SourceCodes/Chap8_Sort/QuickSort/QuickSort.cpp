#include <iostream>
#include <stdlib.h>
using namespace std;
#include "../sort.h"

int SelectPivot(int left, int right)  {				// 选择轴值，参数left,right分别表示序列的左右端下标
	return (left+right)/2;						// 选择中间记录作为轴值
}

template <class Record>
void QuickSort(Record Array[], int left, int right) { 		// Array[]为待排序数组，left,right分别为数组两端
	if (right <= left) 	return;					// 如果子序列中只有0或1个记录，就不需排序
	int pivot = ::SelectPivot(left, right); 			// 选择轴值
	swap(Array, pivot, right);   				// 分割前先将轴值放到数组末端
	pivot = Partition(Array, left, right); 			// 分割后轴值已到达正确位置
    	QuickSort(Array, left, pivot-1);  				// 对轴值左边的子序列进行递归快速排序
    	QuickSort(Array, pivot +1, right); 			// 对轴值右边的子序列进行递归快速排序
}

template <class Record>
int Partition(Record Array[], int left, int right)  {		// 分割函数，分割后轴值已到达正确位置
	int l = left;								//  l为左指针，r为右指针
	int r = right;	
	Record TempRecord = Array[r];				// 将轴值存放在临时变量中
	while (l != r)  {							// 开始分割，l,r不断向中间移动，直到相遇
		// l指针向右移动，直到找到一个大于轴值的记录
		while (Array[l] <= TempRecord && r > l)	// 这里也可以把"<="改写为"<"，反正不稳定
			l++;
		if (l < r)  {						// 若l,r尚未相遇，将逆置元素换到右边的空位
			Array[r] = Array[l];
			r--;							//  r指针向左移动一步
		}
		// r指针向左移动，直到找到一个小于等于轴值的记录
		while (Array[r] > TempRecord && r > l) 	// 这里也可以把">"改写为">="，减少记录移动
			r--;
		if (l < r)  {						// 若l,r尚未相遇，将逆置元素换到左边的空位
			Array[l] = Array[r];
			l++;							// l指针向右移动一步
		}
	}	//end while
	Array[l] = TempRecord;					// 把轴值回填到分界位置l上
	return l;								// 返回分界位置l
}


template <class Record>
void sort(Record* Array, int n) {
	QuickSort(Array, 0, n-1);
}

#include "../SortMain.h"
