#include <iostream>
#include "HuffmanTree.h"
using namespace std;
int main(){

	int weight[] = {6,2,3,4};       //权值
	
	HuffmanTree<int> a(weight,4);    //图5.19 Huffman树
	cout<< "HuffmanTree is constructed. " << endl;

	cout<< "中序周游： " << endl;
	a.InOrder(a.GetRoot());        //中序周游
	return 0;
}