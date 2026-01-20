#include <iostream>
#include <stdlib.h>
#include "MinHeap.h"
#include "Buffer.h"
#include <memory.h>
using namespace std;
typedef unsigned char BYTE;

#define  MAX   7  // 堆大小

//***************  file operation ****************//
// 函数功能：初始化文件
int initFiles(FILE *&inputFile, FILE *&outputFile, const char *in, const char *out)  {
	if ((inputFile = fopen(in, "rb+")) == NULL)  {
		cout << "Cannot open file!!!" << endl;
		return 0; 
	}
	if ((outputFile = fopen(out, "rb+")) == NULL)  {
		cout << "Cannot open file!!!" << endl;
		return 0;	
	}
	return 1; 
}

// 函数功能：初始化堆里的数据，从磁盘文件读n个数据置入数组A
template <class T>
void initMinHeapArry(FILE *inputFile, int n, T *A)  {
	fread(A, sizeof(BYTE), n, inputFile);
//	cout << "file read!!!" << endl;
}

// 函数功能：初始化input buffer, 读入一部分数据
template <class T>
void initInputBuffer(Buffer<T> &input, FILE *inputFile)  {
	input.L = (BYTE *)malloc(BUFFER_SIZE);
	memset(input.L, NULL, BUFFER_SIZE);
	input.nLen = fread(input.L, sizeof(BYTE), BUFFER_SIZE, inputFile);
//	cout << "InputBuffer initialised!!" << endl;
}

template <class T>
void initOutputBuffer(Buffer<T> & output)  {
	output.L = (BYTE *)malloc(BUFFER_SIZE);
}

// 函数功能：把mval输出到输出缓冲区，同时出来因缓冲区空或者满的情况
template <class T>
void sendToOutputBuffer(Buffer<T> &input, Buffer<T> &output, FILE *inputFile, FILE *outputFile, T mval)  {
	// 输出缓冲区满，写文件

	if (output.isFull())  {
		output.flush(outputFile); // 不满，写缓冲
//		cout<<"out buffer is full."<<endl;
	}
	
	output.insert(mval);         // 输入缓冲区空，读文件到缓冲
//	cout<<"output.insert(mval)"<<endl;
	if (input.isEmpty() == true)   {
		if (!feof(inputFile))  {
			// 文件没有结束
			fillBuffer(inputFile, input);
		}
	} 
}

// 函数功能：比较大小
template <class T>
bool less(T r, T mval)  {
	if (r < mval)
		return true;
	else
		return false;
}

// 函数功能：算法结束，处理输入输出缓冲区
template <class T>
void endUp(Buffer<T> output, FILE *inputFile, FILE *outputFile)  {
	fwrite(output.L, sizeof(BYTE), output.nLen, outputFile);
	fclose(inputFile);
	fclose(outputFile);
	cout << "output file." << endl;
}

template <class T>
void replacementSelection(T * A, int n, const char * in, const char * out)  {
	T mval;  //存放最小值堆的最小值
	T r;     //存放从输入缓冲区中读入的元素	
	FILE * inputFile;                   //输入文件句柄
	FILE * outputFile;                  //输出文件句柄	
	Buffer<T> input;                   //输入buffer
	Buffer<T> output;                  //输出buffer	

	initFiles(inputFile, outputFile, in, out);  //初始化输入输出文件
	//初始化堆的数据，从磁盘文件读入n个数据置入数组A
	initMinHeapArry(inputFile, n, A);  //从磁盘文件读入n个数据到堆数组A
	MinHeap<T> H(A, n);          //建立最小值堆
	initInputBuffer(input, inputFile);    //初始化input buffer，读入一部分数据
	initOutputBuffer(output);	  //初始化Output buffer

	for(int last = (n-1); last >= 0;)  {   //堆不为空，就做这个循环
		mval = H.heapArray[0];       //堆的最小值
		//mval = H.RemoveMin();
		//把mval送到输出缓冲区，同时处理因缓冲区空或满造成的各种情形
	
		sendToOutputBuffer(input, output, inputFile, outputFile, mval);

		input.read(r);  //从输入缓冲区读入一个记录

		if(!less(r, mval))  {    // r值大于等于刚输出值，r入堆根
				H.heapArray[0] = r;
		}
		else  {                  // r不能入堆，暂存待处理
			H.heapArray[0] = H.heapArray[last];  // 用last位置的记录代替根结点
			H.heapArray[last] = r;				// 把r放到last位置
			H.setSize(last);					// 堆规模缩小1
			last--;
		}		
		if(last != 0)  {       //重新排列堆
			H.SiftDown(0);  //把根结点记录下降到合适的位置
		}

	}//endfor

	//算法结束工作：处理输出缓冲区，输入/输出文件
	endUp(output, inputFile, outputFile);
}


void main()  {
	char in[] = "file_in.txt";   // 输入文件名称
	char out[] = "file_out.txt"; // 输出文件名称
	int  n = MAX; // 数组元素个数，即内存数目 
	BYTE *A = new BYTE[MAX];
	replacementSelection(A, n, in, out);
}