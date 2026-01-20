#include "MinHeap.h"
#include <iostream>

using namespace std;
int main()  {

	int temp,elem;

	MinHeap<int> minheap(8);
	int i;
/*
	int Key[] = {19,8,35,65,40,3,7,45} ;    //图例5.17所示的数组
	
	cout << "建堆：" << endl;    //此处为依次插入，不同于图示，图示方法可以直接在构造函数里写
	for(i = 0;i < 8; i++)  {            //建堆过程
		minheap.Insert(Key[i]);
		cout<< Key[i] << " added." <<endl;
	}
*/

	cout<< "删除给定下标(3)的元素:" <<endl;
	minheap.Remove(3,elem);
	cout<<  elem << " deleted." <<endl;

	//插入45
	minheap.Insert(45);


	cout << "从堆顶删除最小值:" << endl;
	for (i = 0; i <8; i ++)  {           //依次从堆顶删除最小值
		temp = minheap.RemoveMin();
		cout << temp << " ";             //打印当前最小值
	} 
	cout << "\n\n";
	
	return 0;	
} 