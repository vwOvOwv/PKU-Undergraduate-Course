// abstract data type for List 

template <class T> 
class List  {
      void clear(); 				 	// 置空线性表
      bool isEmpty();				 	// 线性表为空时，返回True
      bool append(T value);		 		// 在表尾添加一个元素value，表的长度增1
      bool insert(int p, T value);	 		// 在位置p上插入一个元素value，表的长度增1
      bool del(int p); 			 	// 删除位置p上的元素，表的长度减 1
      T getValue(int p);			 	// 返回位置p的元素值 
      T setValue(int p, T value);	 		// 用value修改位置p的元素值
}; 
