#include <iostream>
#include <stdlib.h>
using namespace std;
#include "../sort.h"

// 针对16位的数据，< 32003 的数据
/*
// 二进制，16趟
#define radix 2
#define DStep 16
*/

/*
// 四进制，8趟
#define radix 4
#define DStep 8
*/

/*
// 八进制，6趟
#define radix 8
#define DStep 6
*/

// 十六进制，4趟
#define radix 16
#define DStep 4

/*
// 三十二进制，4趟
#define radix 32
#define DStep 4
*/


template <class Record>
void RadixSort(Record Array[], int n, int d, int r)  {
	//数组实现的基数排序，n为数组长度，d为排序码个数，r为基数
	Record *TempArray =new Record[n];		// 辅助排序的临时数组
	int* count = new int[r];				// 桶容量计数器，count[i] 存储了第i个桶中的记录数
	int i,j,k;
	int Radix = 1;							// 模保持进位，用来取Array[j]的第i位排序码
	for (i=0; i<d; i++)  {					// 分别对第i个排序码进行分配
		for (j=0; j<r; j++)					// 初始计数器均为0
			count[j] = 0;    
		for (j=0; j<n; j++)	 {				// 统计每个桶中的记录数
			k = (Array[j] / Radix) % r;		// 取Array[j]的第i位排序码
			count[k]++;						// 相应计数器加1
		}
		for (j=1; j<r; j++)					// 将TempArray中的位置依次分配给r个桶
			count[j] = count[j-1] + count[j];
		for (j=n-1; j>=0; j--) {			// 从数字尾部，把记录收集到相应桶 
			k = (Array[j] / Radix ) % r;	// 取Array[j]的第i位排序码
			count[k]--;						// 使用了第k个桶的一个位置，桶剩余量的计数器减1
			TempArray[count[k]] = Array[j];
		}
		for (j=0; j<n; j++)					// 将临时数组中的内容复制到Array中
			Array[j] = TempArray[j];         
		Radix *= r;							// 往左进一位，修改模Radix
	}
}

template <class Record>
void sort(Record* Array, int n) {
    RadixSort(Array, n, DStep, radix);    // 16进制
  // RadixSort(Array, n, 6, 8);		// 8进制
}

#include "../SortMain.h"
