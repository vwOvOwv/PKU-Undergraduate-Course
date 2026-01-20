#include <iostream>
#include <stdlib.h>
using namespace std;
#include "../sort.h"


template <class Record>
void InsertSort(Record Array[], int n)  {
	// 直接插入排序，Array[]为待排序数组，n为数组长度
	Record TempRecord;						// 临时变量
	for (int i=1; i<n; i++)  {					// 依次插入第i个记录
		TempRecord=Array[i];
		int j = i-1;
		//从i开始往前寻找记录i的正确位置
		while (j >= 0 && TempRecord < Array[j])  {
			Array[j+1] = Array[j];				// 将那些大于等于记录i的记录后移
			j = j - 1;
	    }
	    Array[j+1] = TempRecord; 				// 此时j后面就是记录i的正确位置，回填
	}
}

int SelectPivot(int left, int right)  {			// 选择轴值，参数left,right分别表示序列的左右端下标
	return (left+right)/2;					// 选择中间纪录作为轴值
}

template <class Record>
void ModQuickSort(Record Array[], int left, int right) {  // 优化的快速排序，
	if (right <= left)  return;			// 如果序列中只有0或1个记录，就不用排序
	if (right-left+1 > THRESHOLD)  {		// 对长度大于阈值(28最佳)的长子串处理
	int pivot = ::SelectPivot(left, right); 		// 选择轴值	
	swap(Array, pivot, right);   		// 将轴值放在数组末端
	pivot = Partition(Array, left, right); 		// 分割后轴值已到达正确位置
	ModQuickSort(Array, left, pivot-1);  		// 对轴值左边的子序列进行递归快速排序
	ModQuickSort(Array, pivot+1, right); 		// 对轴值右边的子序列进行递归快速排序
	}
}


template <class Record>
int Partition(Record Array[], int left, int right)  {		// 分割函数，分割后轴值已到达正确位置
	int l = left;								//  l为左指针，r为右指针
	int r = right;	
	Record TempRecord = Array[r];				// 将轴值存放在临时变量中
	while (l != r)  {							// 开始分割，l,r不断向中间移动，直到相遇
		// l指针向右移动，直到找到一个大于轴值的记录
		while (Array[l] <= TempRecord && r > l)	// 这里也可以把“<=”改写为“<”，反正不稳定
			l++;
		if (l < r)  {						// 若l,r尚未相遇，将逆置元素换到右边的空位
			Array[r] = Array[l];
			r--;							//  r指针向左移动一步
		}
		// r指针向左移动，直到找到一个小于等于轴值的记录
		while (Array[r] > TempRecord && r > l) 	// 这里也可以把“>”改写为“>=”，减少记录移动
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
	ModQuickSort(Array, 0, n-1);			// 调用优化的递归快排，不处理小子串
	InsertSort(Array, n);				// 最后这个序列进行扫尾插入排序
}


#include "../SortMain.h"
