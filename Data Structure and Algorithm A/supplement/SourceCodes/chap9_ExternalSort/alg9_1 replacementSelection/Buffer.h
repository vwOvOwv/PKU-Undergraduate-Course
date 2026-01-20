/***************  Buffer.h  *****************/
#include <iostream>
#include <stdio.h>
using namespace std;
#define BUFFER_SIZE  100  // 缓冲区大小

template <class T>
class Buffer  {
public:
	T *L;        // 缓冲区
	int nLen;    // 缓冲大小
	int nCurr;   // 当前指针，初始化为0
public:
	Buffer() {L = NULL; nLen = 0; nCurr = 0;};  //构造函数
	bool isEmpty();                             //空否
	bool isFull();                              //满否
	
	void insert(T e);						    //插入数据
	void read(T &e);                            
	void flush(FILE *fp);                       //将缓冲区写回到文件
};

template <class T>
bool Buffer<T>::isEmpty()  {  //空否
	if (nCurr == nLen)
		return true; 
	return false;
}

template <class T>
bool Buffer<T>::isFull()  {   //满否
	if (nLen == BUFFER_SIZE) 
		return true; 
	return false;
}

template <class T>
void Buffer<T>::insert(T e)  {  //插入数据
	if (nLen < BUFFER_SIZE)
		L[nLen++] = e;
// 	cout<<"buffer insert!"<<endl;
}

template <class T>
void Buffer<T>::read(T &e)  {     //读出数据
	e = L[nCurr++];
//	cout<< "Buffer read out."<<endl;
}

template <class T>
void Buffer<T>::flush(FILE *fp)  {  //将缓冲区写回到文件
	fwrite(L, sizeof(T), nLen, fp); 
	nLen = 0;
	nCurr = 0;
}

template <class T>
void fillBuffer(FILE *fp, Buffer<T> b)  {    //填充数据
	b.nLen = fread(b.L, sizeof(T), BUFFER_SIZE, fp);
	b.nCurr = 0;
}

