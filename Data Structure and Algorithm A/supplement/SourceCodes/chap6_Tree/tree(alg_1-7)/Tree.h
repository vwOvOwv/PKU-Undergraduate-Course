//#include <queue>
#include <iostream>
using namespace std;

template  <class T>
class Tree  {
private:
	TreeNode<T>* root;                         //树根结点
	void DestroyNodes(TreeNode<T>*root);       //删除以root为根的子树
public:
	Tree();										//构造函数
	virtual ~Tree();							//析构函数
	TreeNode<T>* getRoot();						//返回树中的根结点
	void CreateRoot(const T& rootValue);     //创建树中的根结点，使根结点元素的值为rootValue
	bool isEmpty();							 //判断是否为空树，如果是则返回true
	TreeNode<T>* Parent(TreeNode<T>* current);	    //返回current结点的父结点
	TreeNode<T>* PrevSibling(TreeNode<T>* current); //返回current结点的前一个邻居结点
	void DeleteSubTree(TreeNode<T>* subroot);	    //删除以subroot为根的子树的所有结点
    void RootFirstTraverse(TreeNode<T>* root);		//先根深度优先周游树
	void RootLastTraverse(TreeNode<T>* root);		//后根深度优先周游树
	void WidthTraverse(TreeNode<T>* root);			//广度优先周游树
	void Visit(T Value) {							//访问
		cout << Value;
	};           
};

template <class T>
Tree<T>::Tree()	 {							//构造函数
	root=NULL;
}
template <class T>
Tree<T>::~Tree()  {							//析构函数
	while(root)
		DeleteSubTree(root);
}

template <class T>
TreeNode<T>* Tree<T>::getRoot()  {			//返回树中的根结点
	return root;
}
template <class T>
void Tree<T>::CreateRoot(const T& rootValue)  {//创建树中的根结点，使根结点元素的值为rootValue
	if(!root)
		root=new TreeNode<T>(rootValue);
}
template <class T>
bool Tree<T>::isEmpty()  {		 //判断是否为空树，如果是则返回true
	if(root)
		return false;
	return true;
}
template <class T>
TreeNode<T>* Tree<T>::PrevSibling(TreeNode<T>* current)  {//返回current结点的前一个邻居结点
	using std::queue;								//使用STL队列
	queue<TreeNode<T>*> aQueue;
	TreeNode<T>* pointer=root;						//标识当前结点
	TreeNode<T>* prev=NULL;						//标识当前结点的前一个兄弟结点
	//当前结点为空，树为空或所求结点为根结点时，返回NULL	
	if((current==NULL)||(pointer==NULL)||(current==root))
		return NULL;								
	while(pointer)  {
		if(pointer==current)
			return prev;							//找到当前结点
		aQueue.push(pointer);
		prev=pointer;
		pointer=pointer->pSibling;					//沿当前结点右兄弟结点链寻找
	}
	while(!aQueue.empty())  {
		prev=NULL;
		pointer=aQueue.front();				 		
		aQueue.pop();								//出队列
		pointer=pointer->LeftMostChild();			//下降到左子结点
		while(pointer)  {
			if(pointer==current)
				return prev;
			aQueue.push(pointer);
			prev=pointer;
			pointer=pointer->pSibling;				//沿当前结点右兄弟结点链寻找
		}//end while
	}//end while
	return NULL;
}

template  <class T>
TreeNode<T>* Tree<T>::Parent(TreeNode<T>* current)  {
	using std::queue;                          		// 使用STL队列
	queue<TreeNode<T>*> aQueue;
	TreeNode<T>* pointer = root;
	TreeNode<T>* upperlevelpointer = NULL;     		// 用于记录parent结点
	if (current != NULL && pointer != current) {
		while (pointer) {                       		// 森林中所有根结点进队列
			if (current == pointer)
				return NULL;    				// 根的父结点指针为空，返回
			aQueue.push(pointer);
			pointer=pointer-> RightSibling();
		}
		while (!aQueue.empty()) {
			pointer = aQueue.front();            		// 取队列首结点指针
			aQueue.pop();                    		// 出队列	
			upperlevelpointer = pointer;		    		// 指向上一层的结点
			pointer = pointer-> LeftMostChild();  		// 指向当前结点的最左孩子
			while (pointer) {                  		// 当前结点的子结点进队列
				if (current == pointer)
					return upperlevelpointer;    	// 返回父结点指针
				else {
					aQueue.push(pointer);
					pointer = pointer->RightSibling();
				}
			}//end while
		}//end while
	}//end if
	return NULL;
}



template  <class T>
void Tree<T>::DestroyNodes(TreeNode<T>* root)  {
	//删除以root为根的子树的所有结点
	if (root) {
		DestroyNodes(root->LeftMostChild());	//递归删除第一子树
		DestroyNodes(root->RightSibling());		//递归删除其他子树
		delete root;							//删除根结点
	}
}


template  <class T>
void Tree<T>::DeleteSubTree(TreeNode<T>* subroot)  {
	// 删除以subroot为根的子树的所有结点
	if (subroot == NULL) return;
	TreeNode<T>* pointer = Parent (subroot); 	// 找subroot的父结点
	if (pointer == NULL) 							// subroot就是森林第一个树根
		root = subroot->RightSibling();
	else if (pointer-> LeftMostChild() == subroot) 		// subroot为最左子结点的情况
		pointer->setChild(subroot->RightSibling());
	else {									// subroot有左兄弟
		pointer = pointer-> LeftMostChild();		// 下降到最左兄弟
		while (pointer-> RightSibling() != subroot) 	// 顺右链找到直接左兄弟
			pointer = pointer ->RightSibling();
		pointer->setSibling(subroot->RightSibling());
	}
	subroot->setSibling(NULL);
	DestroyNodes(subroot);
}



//树的深度、广度周游算法
template  <class T>
void Tree<T>::RootFirstTraverse(TreeNode<T>* root)  { 	//先根深度优先周游树
	while (NULL != root) {
		Visit(root->Value());					  //访问当前结点
		RootFirstTraverse(root->LeftMostChild());   //周游头一棵树树根的子树
		root = root->RightSibling();			      //周游其他的树
	}
}

template  <class T>
void Tree<T>::RootLastTraverse(TreeNode<T>* root)  { 	//后根深度优先周游树
	while (NULL != root) {
		RootLastTraverse(root->LeftMostChild());  //周游头一棵树树根的子树
		Visit(root->Value());					//访问当前结点
		root = root->RightSibling();			    //周游其他的树
	}
}

template  <class T>
void Tree<T>::WidthTraverse(TreeNode<T>* root)  {  // 广度优先周游树
    using std::queue;                             // 使用STL队列
    queue<TreeNode<T>*> aQueue;
	TreeNode<T>* pointer = root;				// 根作为当前结点
    while (pointer)  {
        aQueue.push(pointer);                  // 当前结点进入队列
        pointer = pointer->RightSibling();         // 指向当前结点的右兄弟
	}//end while
	while (!aQueue.empty()) {
        pointer = aQueue.front();                // 取队列首结点指针
        aQueue.pop();                         // 出队列
        Visit(pointer->Value());                 // 访问当前结点
        pointer = pointer-> LeftMostChild();       // 指向当前结点的最左孩子
        while (pointer) {                       // 当前结点的子结点进队列
            aQueue.push(pointer);
            pointer = pointer->RightSibling();
        }
	}//end while
}
