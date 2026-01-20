#include <iostream>
#include <stdlib.h>
using namespace std;
#include "../sort.h"

template <class Record>
void InsertSort (Record Array[], int n)  {
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

/*
template <class Record>
void MergeSort(Record Array[], Record TempArray[], int left, int right)  {
	//两路归并排序，Array[]为待排序数组，left，right分别为数组两端
	if (left < right)  {							// 如果序列中只有0或1个记录，就不用排序
		int middle=(left+right)/2;					//从中间划分为两个子序列
		MergeSort(Array,TempArray,left,middle);		//对左边一半进行递归
		MergeSort(Array, TempArray,middle+1,right);	//对右边一半进行递归
		Merge(Array, TempArray,left,right,middle);		// 进行归并
	}
}

template <class Record>
void Merge(Record Array[], Record TempArray[], int left, int right, int middle)  {  //归并过程
	for (int j=left; j<=right; j++)   					// 将数组暂存入临时数组
		TempArray[j] = Array[j];
	int index1=left;								// 左边子序列的起始位置
	int index2=middle+1;							// 右边子序列的起始位置
	int i=left;									// 从左开始归并
	while (index1 <= middle && index2 <= right)  {
		//取较小者插入合并数组中
		if (TempArray[index1] <= TempArray[index2])	// 为保证稳定性，相等时左边优先
			Array[i++] = TempArray[index1++];
		else  Array[i++] = TempArray[index2++];
	}
	while (index1 <= middle)						// 只剩左序列，可以直接复制
		Array[i++] = TempArray[index1++];
	while (index2 <= right) 						// 与上个循环互斥，直接复制剩余的右序列
		Array[i++] = TempArray[index2++];			
}
*/

template <class Record>
void ModMergeSort(Record Array[], Record TempArray[], int left, int right) { 
//	if (right <= left)	return;						// 如果只含有一个元素，直接返回		  
	if (right-left+1 > THRESHOLD)  {				//如果序列长度大于阈值(16最佳)，跳出递归
		int middle=(left+right)/2;					// 从中间划分为两个子序列
		ModMergeSort(Array, TempArray ,left,middle);	// 对左边一半进行递归
		ModMergeSort(Array, TempArray ,middle+1,right);	// 对右边一半进行递归
		ModMerge(Array, TempArray ,left,right,middle);	// 进行归并
	}
    else  InsertSort(&Array[left],right-left+1);        	// 若长度小于等于阈值，采用直接插入排序
}

template <class Record>
void ModMerge(Record Array[],Record TempArray[],int left,int right,int middle)  {	//归并过程
	int index1,index2;							// 两个子序列的起始位置
	int i,j,k ;
	for (i=left; i<=middle; i++)						// 复制左边的子序列
		TempArray[i] = Array[i];
	for (j=1; j<=right-middle; j++)					// 复制右边的子序列，但顺序颠倒过来
		TempArray[right-j+1] = Array[j+middle];
	// 开始归并，取较小者插入合并数组中
	for (index1=left, index2=right, k=left;  k<=right;  k++)
		if (TempArray[index1] <= TempArray[index2])	// 为保证稳定性，相等时左边优先
			Array[k] = TempArray[index1++];
		else  Array[k] = TempArray[index2--];
}

template <class Record>
void sort(Record* array, int n) {
	static Record* temp = NULL;
	if (temp == NULL) temp = new Record[n];				// 申请辅助数组
	ModMergeSort(array, temp, 0, n-1);
}

#include "../SortMain.h"
