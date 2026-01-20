#include <cstdlib>
#include <iostream>

#include "myStack.h"

using namespace std;

template <class T> class Link {
public: 
        T  		data;							// 用于保存结点元素的内容
	    Link 	* next;						// 指向后继结点的指针
	
	    Link(const T info, Link* nextValue)  {	// 具有两个参数的Link构造函数
		           data = info;
		           next = nextValue;
       }
       Link(Link* nextValue = NULL)  {	// 具有两个参数的Link构造函数
		           next = nextValue;
       }
};

template <class T> 
class lnkStack : public Stack <T>  { 
private: 	                    		// 栈的链式存储
   Link<T>*	top;					// 指向栈顶的指针
   int 		size;					// 存放元素的个数
public:                      			// 栈运算的链式实现
    lnkStack(int defSize) {				// 构造函数
    	top = NULL;
    	size = 0;
    }
    ~lnkStack() {					// 析构函数
    	clear();
    }
    void clear() {					// 清空栈内容
    	while (top != NULL) {
    		Link<T> *tmp = top; 
    		top = top->next;
    		delete tmp;
        }
    	size = 0; 
    }
    bool push(const T item) { 	    		// 入栈操作的链式实现	
         Link<T>* tmp = new Link<T>(item, top);
         top = tmp;
         size++;
         return true;
    }
    bool pop(T* item) { 				// 出栈的链式实现
         Link<T> *tmp;
         if (size == 0) {
            cout << "栈为空，不能出栈操作" << endl; 
            return false;
         } 
         *item = top->data; 
         tmp = top->next;
         delete top;
         top = tmp;
         size--;
         return true;
    }
    bool getTop(T* item) {				// 返回栈顶内容，但不弹出
         if (size == 0) {
            cout << "栈为空，不能出栈操作"<< endl; 
            return false;
         } 
         *item = top->data;
         return true;
    }	       
};

