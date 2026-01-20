#include <iostream>
#include "Link.h"
#include "myQueue.h"

template <class T> 
class lnkQueue: public Queue <T> { 
private: 	
	int 		size; 							// 队列中当前元素的个数
	Link<T>* front;							// 表示队头的指针
	Link<T>* rear;							// 表示队尾的指针
public: 									// 队列的运算集
 	lnkQueue(int size)  {    					// 创建队列的实例
		size = 0;
		front = rear = NULL;
}
   	~lnkQueue()  {  						// 消除该实例，并释放其空间
		clear();
}
void clear() {							// 清空队列
	while (front != NULL) {
rear = front;
front = front->next;
delete rear;
			}
			rear = NULL; 
			size = 0;
}
bool enQueue(const T item) { 				//  item入队，插入队尾
		if (rear == NULL) 	{				// 空队列
			front = rear = new Link<T> (item, NULL);
		}
		else  {							// 添加新的元素
			rear-> next = new Link<T> (item, NULL);  
			rear = rear ->next;
		}
		size++;
		return true;
	}
 	bool deQueue(T* item)  {          			// 返回队头元素并从队列中删除
		Link<T> tmp;
		if (size == 0)  {						// 队列为空，没有元素可出队
			cout << "队列为空" << endl;
			return false;
		}
		item = front -> element; 
		tmp = front;
		front = front -> next;
		delete tmp;
		if (front == NULL)
rear = NULL;
size--;
		return true;
 	}
bool front(T* item)  {	       				// 返回队头元素，但不删除
		if (size == 0)  {						// 队列为空，没有元素可出队
			cout << "队列为空" << endl;
			return false;
		}
		item = front -> element; 
		return true;
	}
}
