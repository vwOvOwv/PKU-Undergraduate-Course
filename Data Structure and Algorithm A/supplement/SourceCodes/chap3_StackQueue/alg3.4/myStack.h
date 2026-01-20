// abstract data type for stack
 
template <class T> 					// 栈的元素类型为 T
class Stack { 
public: 	            			// 栈的运算集
	void clear();	   			// 变为空栈
   	bool push(const T item); 		// item入栈，成功则返回真，否则返回假
 	bool pop(T& item);				// 返回栈顶内容并弹出，成功返回真，否则返回假,
 	bool getTop(T& item);	  			// 返回栈顶内容但不弹出成功返回真，否则返回假,
   	bool isEmpty(); 	   			// 若栈已空返回真
   	bool isFull();        			// 若栈已满返回真
};


