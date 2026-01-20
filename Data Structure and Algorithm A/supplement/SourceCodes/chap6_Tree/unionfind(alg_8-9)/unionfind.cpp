#include <iostream>
#include "ParTreeNode.h"
#include "ParTree.h"
using namespace std;
#define  N 10                              // 树结点个数

char strInfo[N] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};  //结点信息数组

// 函数功能：根据字符信息获得数字在数组中的位置下标
int GetNum(char ch)  {
	for (int i = 0; i < N; i ++)
		if (ch == strInfo[i])
			return i;
	return -1;
}

// 函数功能：显示数据在父指针表示法中的父结点下标
void Display(ParTree<char> &aParTree)  {
	for (int i = 0; i < N; i ++)  {
		if (aParTree.array[i].getParent() == NULL)            //如果是没有父结点就用*表示
		cout << "*" << " ";
		else  {
			char ch = aParTree.array[i].getParent()->getValue(); //如果有父结点就打印数组中的父结点的下标
			cout << GetNum(ch) << " ";
		}
	}
	cout << endl;
}


#define PATHCOMPRESSION   1  // 0-不带压缩
                             // 1-带压缩

void main()  {
	ParTree<char> aParTree(N);
	cout << "* means that the node has no parents!\n" ;

	for (int i = 0; i < N; i++)
		cout << i << " ";
	cout << endl;
	int i;
	for (i = 0; i < N; i++)  {
		aParTree.array[i].setValue(strInfo[i]);
		cout << strInfo[i] << " ";
	}
	cout << endl;

	cout << "Union: (A,B),(C,D),(E,F),(G,H),(I,J)\n" ;
	aParTree.Union(GetNum('A'), GetNum('B')); 
	aParTree.Union(GetNum('C'), GetNum('D'));
	aParTree.Union(GetNum('E'), GetNum('F'));
	aParTree.Union(GetNum('G'), GetNum('H'));
	aParTree.Union(GetNum('I'), GetNum('J'));
	Display(aParTree);                             //显示数据在父指针表示法中的父结点下标

	cout << "\nUnion: (A,D),(I,H)\n";
	aParTree.Union(GetNum('A'), GetNum('D'));
	aParTree.Union(GetNum('I'), GetNum('H'));
	Display(aParTree);								//显示数据在父指针表示法中的父结点下标
 
	#if !PATHCOMPRESSION 
	// 0.不带压缩
		cout << "\nUnion: (F,J)\n" ;
		aParTree.Union(GetNum('F'), GetNum('J'));
		Display(aParTree);								//显示数据在父指针表示法中的父结点下标
		cout << "Union: (D,J)\n";
		aParTree.Union(GetNum('D'), GetNum('J'));
		Display(aParTree);								//显示数据在父指针表示法中的父结点下标

	#else
	// 1.带压缩 
		cout << "\nUnion: (F,J)\n";
		aParTree.UnionPC(GetNum('F'), GetNum('J'));
		Display(aParTree);								//显示数据在父指针表示法中的父结点下标
		cout << "\nUnion: (D,J)\n";
		aParTree.UnionPC(GetNum('D'), GetNum('J'));
		Display(aParTree);								//显示数据在父指针表示法中的父结点下标
	#endif

	if (aParTree.Different(GetNum('D'), GetNum('E')))
		cout << "D and E are in different sets!!!" << endl;
	else
		cout << "D and E are in a common set!!!" << endl;
	cout << endl;
} 

