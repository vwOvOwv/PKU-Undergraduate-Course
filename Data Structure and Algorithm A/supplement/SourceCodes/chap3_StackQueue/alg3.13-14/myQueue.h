template <class T> 
class Queue  { 
public: 			        		// 队列的运算集
void clear();					// 变为空队列
bool enQueue(const T& item); 	// item入队，插入队尾，成功则返回真否则返回假
bool deQueue(T& item);     	// 返回队头元素并从队列中删除，成功则返回真
bool front(T& item);	       	// 返回队头元素，但不删除，成功则返回真
bool isEmpty(); 				// 返回真，若队列已空
bool isFull();       			// 返回真，若队列已满
};
