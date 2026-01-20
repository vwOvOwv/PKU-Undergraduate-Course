
template <class T> class Link {
public: 
    	T  		data;							// 用于保存结点元素的内容
	Link 	* next;						// 指向后继结点的指针
	
	Link(const T info, Link* nextValue = NULL)  {	// 具有两个参数的Link构造函数
		data = info;
		next = nextValue;
	}
	Link(Link* nextValue = NULL)  { 			// 具有一个参数的Link构造函数
		next = nextValue;
	}
};

