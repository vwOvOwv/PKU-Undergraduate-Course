// 广义表的类实现
#include "GenList.h"



//GenListNode的构造函数
/*
template<class T>
GenListNode<T>::GenListNode(GenListNode<T> *list)
{
	//
	type=LIST;
    child=list;
	next=NULL;
	mark=0;
	ref=0;
}


template <class T>
GenListNode<T>::GenListNode(T element)
{
	    type=ATOM;
		child=NULL;
		this->element=element;
		next=NULL;
		mark=0;
		ref=0;
}
*/


template <class T>
GenListNode<T>::GenListNode()
{
	    type=LIST;
		child=NULL;
}
//GenListNode的析构函数
template <class T>
GenListNode<T>::~GenListNode()
{
	//free all the memery 
    Destroy(this);
}

//释放内存
template <class T>
void GenListNode<T>::Destory(GenListNode<T> *node)
{
	if(node->next!=NULL)
		Destroy(node->next);
	if(node->child!=NULL)
		Destroy(node->child);
	delete node;
}

/*
template <class T>
void GenListNode<T>::Search(GenListNode<T> *node,GenListNode<T> *prenode,int &flag) 
{   //flag 为回溯标志
	if (node->type==HEAD)	{//进入一个表头结点，准备访问它的子表
		node->mark=VISITED;
		Search(node->next,node,flag);
	}
	else	{//非表头结点，访问每一个element结点
      if (node->type==ATOM) {
		cout<<node->element;
		node->mark=VISITED;
	    } 
      else if(node->type==LIST) {
			node->mark=VISITED;
			cout<<"(";
			Search(node->child,node,flag);
			cout<<")";
		}
      if((node->next!=NULL)&&(node->next->mark!=VISITED)) {//没有访问过，则继续访问
		  cout<<",";
		  node->mark=VISITED;
		  Search(node->next,node,flag);
		}
	  else if((node->next!=NULL)&&(node->type==ATOM))  //否则访问过，回溯
		      flag=BACKTRACE; //mark==VISITED，需要进行回溯
	
	  if(flag==BACKTRACE)	{//回溯操作
		  node->mark=UNVISITED;
		  if(prenode==NULL) 	{//回溯到第一个结点，结束
			 flag=NOTBACKTRACE;
			 return;
		  }
		  if((prenode->child=node)&&(prenode->next!=NULL)&&(prenode->next->mark==0)) {//回溯到可以进入其他子链的地方
			 flag=NOTBACKTRACE;
			 return;
		  }
	   }	
	}
	//将刚才进入子链的mark值重新置为UNVISITED
	GenListNode<T>*temp=node;
	while(temp!=NULL)	{
		temp->mark=UNVISITED;
		temp=temp->next;
	}
}
*/

template <class T>
void GenListNode<T>::GenListTraversalHelp(GenListNode<T> *node) 
{   
	GenListNode<T> *p;
	node->headNode.mark=VISITED;
	cout << "(";
	for (p = node->next; p!=NULL; p=p->next) {
		//p->mark=VISITED;
		if ((p->type==LIST)&&(p->child!=NULL)) {	//进入一个表头结点，准备访问它的子表
			   cout << p->child->headNode.Name;
			   if (p->child->headNode.mark == UNVISITED)  {
				  if (p->child->headNode.Name[0]!='\0')
				  cout <<":";	
			      GenListTraversalHelp(p->child);
			   }
		}
		else if (p->type==ATOM) 
		    cout<<p->element;
		if ((p->next!=NULL)&&(p->next->type!=HEAD))
		    cout << ", ";
	}
	cout << ")";
}

/*
template <class T>
void GenListNode<T>::GenListTraversalHelp(GenListNode<T> *node) 
{   //flag 为回溯标志
	GenListNode<T> *p;
	node->mark=VISITED;
	cout << "(";
	for (p = node->next; (p!=NULL)&&(p->type!=HEAD); p=p->next) {
		p->mark=VISITED;
		if ((p->type==LIST)&&(p->child!=NULL)) {	//进入一个表头结点，准备访问它的子表
			   cout << p->child->headNode.Name;
			   if (p->child->mark == UNVISITED)  {
				  if (p->child->headNode.Name[0]!='\0')
				  cout <<":";	
			      GenListTraversalHelp(p->child);
			   }
		}
		else if (p->type==ATOM) 
		    cout<<p->element;
		if ((p->next!=NULL)&&(p->next->type!=HEAD))
		    cout << ", ";
	}
	cout << ")";
}
*/

template <class T>
void GenListNode<T>::ViewAll()
{
	int flag=NOTBACKTRACE;
	cout<<"(";
	GenListTraversal(this,NULL,flag);
	cout<<")";
}
//广义表的构造函数

template <class T>
GenList<T>::GenList(char *Name)
{
	head= new GenListNode<T>();
	head->next=NULL;
	head->type=HEAD;
	head->headNode.ref=0;
	head->headNode.Name = Name;
	current=head;
}


//广义表的插入函数
template <class T>
void GenList<T>::Insert(T element)
{
	GenListNode<T> *temp=current;
	current=new GenListNode<T>();
	current->type=ATOM;
	current->child=NULL;
	current->element=element;
	temp->next=current;
	current->next=NULL;

}

template <class T>
void GenList<T>::Insert(GenList<T> *genlist)
{
	GenListNode<T> *temp=current;
	GenListNode<T> *n=genlist->GetHead();
	n->headNode.mark=UNVISITED;
	n->headNode.ref++;
   	current=new GenListNode<T>();
	current->child=n;
	current->next=NULL;
	current->type=LIST;
	temp->next=current;
}

//得到头指针
template <class T>
GenListNode<T>* GenList<T>::GetHead()
{
	return head;
}

//得到下一个结点
template <class T>
GenListNode<T>* GenList<T>::GetNext()
{
	if(current->next!=NULL)
	{
		current=current->next;
		return current;
	}
	else return NULL;
}
//得到当前结点的前一个结点
template <class T>
GenListNode<T>* GenList<T>::GetPrev()
{
	GenListNode<T> *temp=head;
	while(temp->next!=NULL)
	{
		temp=temp->next;
		if(temp->next==current)
			return temp;
	}
	return NULL;
}
//删除当前结点,当前结点指针指向下一个结点
template <class T>
int GenList<T>::Remove()
{
	if(current==NULL)
		return -1;
	if(current==head)
		return -2;
	GenListNode<T> *prev=GetPrev();
	GenListNode<T> *next=GetNext();
	prev->next=next;
    GenListNode<T> *temp=current;
	if(next==NULL)
		current=prev;
	else current=next;
	temp->Destory();

}
//move current point to head point
template <class T>
void GenList<T>::MoveToFirst()
{
	current=head;
}


//析构函数
template <class T>
GenList<T>::~GenList()
{
    current=head->next;
	while(current!=head)
		Remove();
    delete head;
}

template <class T>
void GenList<T>::View()
{
//	cout<<"(";
	MoveToFirst();
//	GenListNode<T> *temp=GetNext();
	int first=1;
    current->ViewAll();
		//temp=GetNext();

//	cout<<")";
}


template <class T>
void GenList<T>::ViewList()
{
	MoveToFirst();
	int first=1;
    current->GenListTraversal();
}


template <class T>
void GenListNode<T>::GenListTraversal()
{
	GenListTraversalHelp(this);
}