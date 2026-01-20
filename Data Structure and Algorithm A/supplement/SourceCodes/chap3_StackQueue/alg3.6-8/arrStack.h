//  array-based stack: definition and implementation for some methods

#include <iostream>
#include "myStack.h"

using namespace std;

template <class T> 	
class arrStack : public Stack<T> {      
private: 	                    	// 栈的顺序存储
		int		mSize;			// 栈中最多可存放的元素个数
		T 		*st;	  		// 存放栈元素的数组
public:  
         int		top;					// 栈顶位置，应小于mSize 
public:  						// 栈的运算的顺序实现
	arrStack(int size){ 				// 创建一个顺序栈的实例
		mSize = size;
		top = -1;
		st = new T[mSize];
	}
	arrStack(){
        top = -1;
    }
	~arrStack() {					// 析构函数
		delete [] st;
	}
	void clear() {					// 清空栈内容
		top = -1;
    }
	bool push(const T item) { 	   		// 入栈操作的顺序实现	
		if (top == mSize-1) {  			// 栈已满 
			cout << "栈满溢出" << endl;
			return false;
		}
		else {					// 新元素入栈并修改栈顶指针
			st[++top] = item;
			return true;
		}
	}
	bool pop(T* item) { 	        	                  	// 出栈的顺序实现
		if (top == -1) {			                       // 栈为空
			cout << "栈为空，不能出栈操作"<< endl; 
			return false;
		}
		else {
			*item = st[top--];	         // 返回栈顶元素并修改栈顶指针
			return true;
		}
	}
	bool getTop(T* item) {				     // 返回栈顶内容，但不弹出
		if (top == -1) {			         // 栈空
			cout << " 栈为空，不能出栈操作"<< endl; 
			return false;
		}
		else { 						
			*item = st[top];
			return true;
		}
    }
    bool isEmpty() { 
         return (top == -1);
    }
    bool isFull() { 
         return (top == mSize-1);
    }
};

