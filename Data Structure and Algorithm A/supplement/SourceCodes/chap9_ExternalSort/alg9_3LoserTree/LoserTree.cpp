#include <iostream>
#include "LoserTree.h"
using namespace std;
#define MAX 8 // 选手数

void main()
{
	int A[MAX + 1] =  {0, 10, 9, 6, 20, 12, 8, 15, 16}; //从下标1开始
	LoserTree<int> aLosTree(MAX);
	
	// 输出选手信息
	cout << "Players numbers: " << MAX << endl;
	cout << "Players valus  : ";
	for (int i = 1; i <= MAX; i ++)
		cout << A[i] << " ";
	cout << endl << endl;
	
	// 初始化
	aLosTree.Initialize(A, MAX, winner, loser);
	cout << "The loser tree is" << endl;
	aLosTree.Output();
	
	// 选手3获胜，改为11后结果
	A[3] = 11;
	aLosTree.RePlay(3, winner, loser);
	cout << "Changed player 3 to 11, new tree is" << endl;
	aLosTree.Output();
}
