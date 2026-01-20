

// array-base queue
template <class T> 
class arrQueue: class Queue<T> { 
private: 	
	int 	mSize; 						// 存放队列的数组的大小
	int 	front;							// 表示队头所在位置的下标
	int 	rear;							// 表示队尾所在位置的下标
	T 	*qu；						// 存放类型为T的队列元素的数组
public: 								// 队列的运算集
   	arrQueue(int size)  {    				// 创建队列的实例
		mSize = size +1;					// 浪费一个存储空间，以区别队列空和队列满
		qu = new T [mSize];
		front = rear = 0;
	}
   	~arrQueue()  {  					// 消除该实例，并释放其空间
		delete [] qu;
	}
	void clear {						// 清空队列
		front = rear; 
	}
   	bool enQueue(T item)  { 				//  item入队，插入队尾
		if (((rear + 1 ) % mSize) == front) {
			cout << "队列已满，溢出" << endl;
			return false;
		}
		qu[rear] = item;
		rear = (rear +1) % mSize;  			// 循环后继
		return true;
	}
 	bool deQueue(T& item)  {     			// 返回队头元素并从队列中删除
		if ( front == rear)  {
			cout << "队列为空" << endl;
			return false;
		}
		item = qu[front];
		front = (front +1) % mSize;
		return true;
 	}
	bool front(T & item) {	     			// 返回队头元素，但不删除
		if (front == rear)  {
			cout << "队列为空" << endl;
			return false;
		}
		item = qu[front];
		return true;
	}
}

template <class T> 
class lnkQueue: class Queue <T> { 
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
	void clear {							// 清空队列
		while (front != NULL) {
			rear = front;
			front = front->next;
			delete rear;
		}
		rear = NULL; 
		size = 0;
	}
	bool enQueue(const T& item) { 				//  item入队，插入队尾
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
 	bool deQueue(T & item)  {          		// 返回队头元素并从队列中删除
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
	bool front(T & item)  {	       			// 返回队头元素，但不删除
		if (size == 0)  {						// 队列为空，没有元素可出队
			cout << "队列为空" << endl;
			return false;
		}
		item = front -> element; 
		return true;
	}
}
