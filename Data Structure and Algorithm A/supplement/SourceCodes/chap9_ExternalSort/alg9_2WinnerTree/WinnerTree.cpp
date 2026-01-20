#include <iostream>
#include "WinnerTree.h"
using namespace std;
//第9.3.3.1小节中示例
void main(void)  {
	int A[MAX + 1] =  {0, 10, 9, 6, 20, 12, 8, 15,16};  //数组下标从1开始
	WinnerTree<int> aWinTree; // 输出选手信息
	cout << "Players numbers: " << MAX << endl;
	cout << "Players valus  : ";
	for (int i = 1; i <= MAX; i ++)
		cout << A[i] << " ";
	cout << endl << endl;
	
	// 初始化
	aWinTree.Initialize(A, MAX, winner);
	cout << "winner tree is" << endl;
	aWinTree.Output();
	
	// 选手3获胜，改为11后结果
	A[3] = 11;
	aWinTree.RePlay(3, winner);
	cout << "Changed player 3 to 11, new tree is" << endl;
	aWinTree.Output();
}