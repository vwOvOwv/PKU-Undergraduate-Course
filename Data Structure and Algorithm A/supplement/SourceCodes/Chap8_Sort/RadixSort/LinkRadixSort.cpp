//基于静态链的基数排序

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "LinkSort.h"
#include "LinkRadixSort.h"
using namespace std;
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

/*
// 六十四进制，3趟
#define radix 64
#define DStep 3
*/

/*
// 128进制，3趟
#define radix 128
#define DStep 3

// 256进制，2趟
#define radix 256
#define DStep 2
*/

template <class Record>
void sort(Record* Array, int n) {
/*	
	int listsize = 9;    
	Node *Array = new Node[9];
	
	Array[0].key = 97;
	Array[1].key = 53;
	Array[2].key = 88;
	Array[3].key = 59;
	Array[4].key = 26;
	Array[5].key = 41;
	Array[6].key = 88;
	Array[7].key = 31;
	Array[8].key = 22;

	cout << "排序前:" << endl;
	Print<Node>(Array, listsize);
	RadixSort<Node>(Array, listsize, DStep, radix);
	cout << "排序后:" << endl;
	Print<Node>(Array, listsize);
*/	
	RadixSort(Array, n, DStep, radix);
}

#include "LinkSortMain.cpp"