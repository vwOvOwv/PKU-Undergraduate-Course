#include <iostream>
#include <stdlib.h>
using namespace std;
#include "sort.h"

/*
template<class Record>
void IndexSort(Record Array[], int n)
{	//Array[]为待排序数组，n为数组长度
    int *IndexArray = new int[n],  TempIndex;
	int i,j;
    for (i=0; i<n; i++) IndexArray[i] = i;
	for (i=1; i<n; i++)    // 依次插入第i个记录
		for (j=i;j>0;j--) //依次比较，发现逆置就交换
			if (Array[IndexArray[j]] < Array[IndexArray[j-1]])  {
				TempIndex=IndexArray[j];
                IndexArray[j]=IndexArray[j-1];
			    IndexArray[j-1]=TempIndex;	
			}
			else break;	//此时i前面记录已排序

    // 根据IndexArray整理Array
    for(i=0; i<n; i++) { 
        j=IndexArray[i];
		while (j<i)  
			j=IndexArray[j]; 	
	    swap(Array, i, j);
		IndexArray[i]=j;
	}
}
*/

// Array[]为待排序数组，IndexArray[]为索引数组，n为数组长度
template<class Record>
void IndexSort(Record Array[], int IndexArray[], int n)  {
	int i,j;
    for (i=0; i<n; i++) IndexArray[i] = i;	// 初始化索引下标
    // 简单插入排序的地址排序过程
	for (i=1; i<n; i++)				// 依次插入第i个记录
		for (j=i;j>0;j--)				// 依次比较，发现逆置就交换
			if (Array[IndexArray[j]] < Array[IndexArray[j-1]])
	            swap(IndexArray, j, j-1);  // 交换Array[IndexArray[j]]和Array[IndexArray[j-1]
			else break;				// 此时i前面记录已排序
	AdjustRecord(Array, IndexArray, n); 
}


// 根据IndexArray整理Array，IndexArray[]为索引数组，n为数组长度
template<class Record>
void AdjustRecord(Record Array[],int IndexArray[], int n)  {
	Record TempRec;				// 只需要一个临时存储空间
    for (int i=0; i<n; i++)  {			// 循环n-1次，每次处理数组中第i个记录
		int j = i;					// j为临时变量，是循环链中的当前元素
		TempRec = Array[i];			// 暂存i下标中目前的纪录
		while (IndexArray[j] != i)  {	// 如果循环链中索引下标还不是i，则顺链循环调整
			int k = IndexArray[j];		// k为链接j指向的下标
			Array[j] = Array[k];		// 把k下标中的值复制到j位置，第j大元素正确归位
			IndexArray[j] = j;		// 因为是正确归位，索引j就是自身
			j = k;					// j换到下一个，继续处理
		}
		Array[j]=TempRec;			// 最后找到的j，它的索引下标值为i，因此第i大元素正确入位
		IndexArray[j]=j;				// 因为是正确归位，索引j就是自身
	}
}

template <class Record>
void sort(Record* Array, int n) {
    Record* temp = new Record[n];			// 申请辅助数组
	IndexSort(Array, temp, n);
}

#include "SortMain.h"