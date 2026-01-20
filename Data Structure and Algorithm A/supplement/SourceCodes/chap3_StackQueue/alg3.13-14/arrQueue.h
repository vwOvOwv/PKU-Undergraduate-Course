#include <cstdlib>
#include <iostream>

#include "myQueue.h"

using namespace std;

template <class T> 
class arrQueue: public Queue<T> { 
private: 	
	int 	mSize; 						// 存放队列的数组的大小
	int 	front;							// 表示队头所在位置的下标
	int 	rear;							// 表示队尾所在位置的下标
	T 	    *qu;							// 存放类型为T的队列元素的数组
public: 								// 队列的运算集
   	arrQueue(int size)  {    				// 创建队列的实例
		mSize = size +1;					// 浪费一个存储空间，以区别队列空和队列满
		qu = new T[mSize];
		front = rear = 0;
}
   	~arrQueue()  {  					// 消除该实例，并释放其空间
		delete [] qu;
}
void clear() {						// 清空队列
	front = rear; 
}
   	bool enQueue(const T item)  { 			//  item入队，插入队尾
		if (((rear + 1 ) % mSize) == front) {
			cout << "队列已满，溢出" << endl;
			return false;
		}
		qu[rear] = item;
		rear = (rear +1) % mSize;  			// 循环后继
		return true;
	}
 	bool deQueue(T*item)  {     			// 返回队头元素并从队列中删除
		if ( front == rear)  {
			cout << "队列为空" << endl;
			return false;
		}
		*item = qu[front];
		front = (front +1) % mSize;
		return true;
 	}
    bool getFront(T* item) {	     			// 返回队头元素，但不删除
		if (front == rear)  {
			cout << "队列为空" << endl;
			return false;
		}
		*item = qu[front];
		return true;
	}
	void print() {	     			// 返回队头元素，但不删除
		if (front == rear)  {
			cout << "队列为空" << endl;
		}
		int p = front;
		while(p != rear ) {
              cout << qu[p] << " ";
              p =( p + 1)  % mSize;  
        } 
        cout << endl;
	}
};



