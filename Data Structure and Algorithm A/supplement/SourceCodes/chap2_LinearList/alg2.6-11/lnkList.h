#include <cstdlib>
#include <iostream>

#include "myList.h" 

using namespace std;

template <class T> 
class Link {
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

// 【代码2.7】 单链表的类型定义
template <class T> 
class lnkList: public List<T>  {
private: 
	Link<T>*  head;                    // 单链表的头指针
    Link<T>*  tail;    				// 单链表的尾指针
	Link<T>* setPos(int p);				// 返回线性表指向第p个元素的指针值     
public: 
	lnkList(int);						// 构造函数
	~lnkList();						// 析构函数
    bool isEmpty(); 					// 判断链表是否为空
    void clear(); 					 // 将链表存储的内容清除，成为空表
	int length();                   	          // 返回此顺序表的当前实际长度
    bool append(T value);				   // 在表尾添加一个元素value，表的长度增1
	bool insert(int p, T value);			// 在位置p上插入一个元素value，表的长度增1
	bool del(int p); 				     // 删除位置p上的元素，表的长度减 1
    bool getValue(int p, T value);	       // 返回位置p的元素值 
	bool getPos(int p, const T value);	// 查找值为value的元素，并返回第1次出现的位置
	void print();
};

template <class T> 
lnkList<T>::lnkList(int defSize){
		head = tail = new Link<T>(9999,NULL);
}

template <class T>
lnkList<T>::~lnkList(){
		Link<T> *tmp;
		head = head->next;
        while (head != NULL) { 
 		      tmp = head;
			  head = head->next;
			  delete tmp;
		}
}

template <class T>  			// 假定线性表的元素类型为T
Link<T>* lnkList<T> :: setPos(int i) {
        Link<T> *p;
        int count = 0;
        if (i == -1) 			       // i为-1则定位到"虚"头结点
		   return head;
        
        p = new Link<T>(head->next);      // 若i为0则定位到第一个结点     
        
	    while (p != NULL && count < i) {
 	           p = p-> next;
  	           count++;
        };
        return p;         		// 指向第 i 结点，i＝0,1,…，当链表中结点数小于i时返回NULL
}
     

template  <class T>  			 		// 线性表的元素类型为T
bool lnkList<T> :: insert(int i, T value)  {
     Link<T> *p, *q;
					
     if ((p = setPos(i-1)) == NULL){ 		// p是第i个结点的前驱
		cout << " the insertion point is illegal"<<endl;
		return false;
    }
    q = new Link<T>(value, p->next);
    p->next = q;
    if (p == tail)						// 插入点在链尾，插入结点成为新的链尾
		tail = q;

    return true;
}

// delete a node from singly linked list
template  <class T>  					// 线性表的元素类型为T
bool lnkList<T>:: del(int i)  {
     Link<T> *p, *q; 
	 if  ((p = setPos(i-1)) == NULL )	{ 		// 待删结点不存在，即给定的i大于当前链中元素个数
		cout << " the deletion point is illegal"<<endl;
		return false;
	}
    q = p->next;							// q是真正待删结点
    if (q == tail) {						// 待删结点为尾结点，则修改尾指针
		  tail = p;
		  delete q;
    }
    if (q != NULL)  {						// 删除结点q 并修改链指针
    	p->next = q->next;
	    delete q;
     }
     return true;
}
 
template <class T>  			    // 假定顺序表的元素类型为T
void lnkList<T> :: print() {
       	Link<T> *tmp = head;
       	cout << "begin\n" ;
        while (tmp != NULL) { 
 	    	cout << tmp->data <<" ";
      	    tmp = tmp->next;
		}
		cout << "\n";
		cout << "end\n";
}
