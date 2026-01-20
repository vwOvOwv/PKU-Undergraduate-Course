#pragma once
//基于静态链的基数排序

//结点类
#define Elem int
class Node {
public:
	Elem key;	//结点的关键码值
	int next;	//下一个结点在数组中的下标
};

//静态队列类
class StaticQueue{  
public:
    int head;
    int tail;
};

//静态链实现的基数排序，n为数组长度，d为排序码个数，r为基数
template <class Record>
void RadixSort(Record* Array, int n, int d, int r) { 
	int first=0;						// first指向静态链中第一个记录
	StaticQueue *queue;
	queue = new StaticQueue[r];			// 存放r个桶的静态队列
	for (int i=0; i<n-1; i++)			// 建链，初始为next域指向下一个记录
		Array[i].next = i+1;
	Array[n-1].next = -1;				// 链尾next为空
	int i;
	for (i=0; i<d; i++)  {			// 对第i个排序码进行分配和收集，一共d趟
		Distribute(Array, first, i, r, queue);
		Collect(Array, first,  r, queue);
	}
	delete[] queue;
//    PrintArray(Array, first);
    AddrSort(Array, n, first);			// 线性时间整理静态链表，使得数组按下标有序
}

// 分配过程,A中存放待排序记录，first为静态链中的第一个记录,i为第i个排序码，r为基数
template <class Record>
void Distribute(Record* Array, int first, int i, int r, StaticQueue* queue)  {
	for (int j=0; j<r; j++)				// 初始化r个队列
		queue[j].head = -1;     
	while (first != -1)  {				// 对整个静态链进行分配	   
		int k = Array[first].key;		// 取第i位排序码数字k
		for (int a=0; a<i; a++)
			k = k / r;
		k = k % r;
		// 把Array[first]分配到第k个子序列中
		if (queue[k].head == -1)		// 如果子序列为空，Array[first]就是第一个记录
			queue[k].head = first;    
		else							// 否则加到子序列的尾部
			Array[queue[k].tail].next = first;  
		queue[k].tail = first;			// first为子序列的尾部
		first = Array[first].next;		// 继续分配下一个记录 
	}
}

// 收集过程，Array中存放待排序记录，first为静态链中的第一个记录，r为基数
template <class Record>
void Collect(Record* Array, int& first, int r, StaticQueue* queue)  {
	int last, k=0;						// 已收集到的最后一个记录
	while (queue[k].head == -1) 		// 找到第一个非空队列
		k++;    
	first = queue[k].head;
	last = queue[k].tail;
	while (k < r-1)  {					// 继续收集下一个非空队列，若k==r-1则已是最后一个	
		// 找下一个非空队列
		k++; 
		while (k < r-1 && queue[k].head == -1) 	// 当前队列k为空，而且k还不是最后的队列r-1
			k++;					// 试探下一个队列
		if (queue[k].head != -1)  {		// 将这个非空序列与上一个非空序列连接起来		
			Array[last].next = queue[k].head;	
			last = queue[k].tail;		// 此时最后一个记录为这个序列的尾部记录
		}
	}
	Array[last].next = -1; 				// 收集完毕
}

template <class Record>
void AddrSort(Record *Array, int n, int first)  {	// 线性时间整理静态链表，使得数组按下标有序
	int i, j;
    j = first;					// j待处理数据下标，第一次为first
	Record TempRec;
	for (i=0; i<n-1; i++)	{			// 循环n-1次，每次处理第i大记录
		TempRec = Array[j];		// 暂存第i大的纪录
		Array[j] = Array[i];			// 当前下标i的数据存放到j位置
		Array[i] = TempRec;		// 第i大记录入位
		Array[i].next = j;			// 第i大的记录的next链要保留调换轨迹j
		j = TempRec.next;			//  j移动到下一位
		while (j <= i)   			// 若j比当前下标i小，则是轨迹，顺链找到数据
			j=Array[j].next;
	}
}
