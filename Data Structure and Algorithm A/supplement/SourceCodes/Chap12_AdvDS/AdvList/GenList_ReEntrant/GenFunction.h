// 广义表的类实现
#include "GenList.h"
#include <string.h>


//GenListNode的构造函数


template <class T>
GenListNode<T>::GenListNode()
{
	    type=LIST;
		child=NULL;
		next=NULL;
		mark=0;
		headNode.VisitTimes=0;
}
//GenListNode的析构函数
template <class T>
GenListNode<T>::~GenListNode()
{
	//free all the memery 

}

//释放内存
template <class T>
void GenListNode<T>::Destory(GenListNode<T> *node)
{
	if(node->next!=NULL)
		Destory(node->next);
	if(node->child!=NULL)
		Destory(node->child);

	  delete node;
}

template <class T>
void GenListNode<T>::Insert(GenListNode<T>*node,GenListNode<T>*current)
{
	GenListNode<T>*tail=this;
	GenListNode<T>*temp=new GenListNode<T>();
	temp->type=LIST;
	temp->child=node;
	node->headNode.ref++;
	while(tail->next!=NULL)
		tail=tail->next;
	tail->next=temp;
	current=temp;
	temp->next=NULL;
	
}

template <class T>
void GenListNode<T>::Insert(T element,GenListNode<T>*current)
{
	GenListNode<T>*tail=this;
	GenListNode<T>*temp=new GenListNode<T>();
	temp->type=ATOM;
	temp->element=element;
	while(tail->next!=NULL)
		tail=tail->next;
	tail->next=temp;
	temp->next=NULL;
	temp->child=NULL;
	
	current=temp;
}

template <class T>
void GenListNode<T>::Search(GenListNode<T> *node,GenListNode<T> *prenode,int VisitTime) 
{   //
	if (node->type==HEAD)	
	{//进入一个表头结点，准备访问它的子表
		node->mark=VISITED;
		if(node->headNode.VisitTimes>VisitTime)
			cout<<node->headNode.Name;
		else
		{
			cout<<node->headNode.Name<<":(";
			node->headNode.VisitTimes++;
			if(node->next!=NULL)
				Search(node->next,node,VisitTime);
			else node->mark=UNVISITED;
			cout<<")";
		}
	}
	else	
	{//非表头结点，访问每一个element结点
      if (node->type==ATOM) {
		cout<<node->element;
		node->mark=VISITED;
	    } 
      else if(node->type==LIST) 
	  {
		
		Search(node->child,node,VisitTime);
	  }
      if((node->next!=NULL)&&(node->next->mark!=VISITED)) 
	  {//没有访问过，则继续访问
		  cout<<",";
		  node->mark=VISITED;
		  Search(node->next,node,VisitTime);
	  }

	}
	//将刚才进入子链的mark值重新置为UNVISITED
	GenListNode<T>*temp=node;
	while(temp!=NULL)	{
		temp->mark=UNVISITED;
		temp=temp->next;
	}
	
}


template <class T>
GenListNode<T>* GenListNode<T>::SearchDelete(char *Name,GenListNode<T>* node)
{
	GenListNode<T>* retval=NULL;
	GenListNode<T>* pre;
	GenListNode<T>* curr;
	GenListNode<T>* temp;
	if(node->mark!=VISITED)//未访问过，必然未删除过
	{
		node->mark=VISITED;
		pre=node;
		curr=node;
		while(curr->next!=NULL)
		{
			curr=curr->next;
			if(curr->type==LIST)
			{
				if(strcmp((curr->child->headNode).Name,Name)==0)
				{
					retval=curr->child;
					curr->child=NULL;
					pre->next=curr->next;
					temp=curr;
					curr=pre;
					delete temp;
				}
				else
					retval=SearchDelete(Name,curr->child);
			}
			pre=curr;
		}
	}
    node->mark=UNVISITED;
	return retval;
}

template <class T>
void GenListNode<T>::Delete(char *Name)
{
	GenListNode<T>* node;
	if(strcmp(Name,(this->headNode).Name)==0)
	{//是根结点
		Destory(this);
	}
	else
	{
		node=SearchDelete(Name,this);
		if(node!=NULL)
		  Destory(node);
	}
}

	
template <class T>
GenListNode<T>* GenListNode<T>::SearchInsert(char *Name,GenListNode<T>* node)
{
	GenListNode<T>* pre;
	GenListNode<T>* curr;
    GenListNode<T>* result=NULL;
	if(node->mark!=VISITED)//如果访问过,则说明要插入的节点不再此处
	{
		node->mark=VISITED;
		pre=node;
		curr=node;
		while(curr->next!=NULL)
		{
			curr=curr->next;
			if(curr->type==LIST)
			{
				if(strcmp((curr->child->headNode).Name,Name)==0)
				{
					
					result=curr->child;
					
				}
				else
				{
					result= SearchInsert(Name,curr->child);
				}
			}
			pre=curr;
		}
	}
	node->mark=UNVISITED;
	return result;
}

template <class T>
void GenListNode<T>::ViewAll()
{
	
	Search(this,NULL,this->headNode.VisitTimes);
	cout<<endl;

}
//广义表的构造函数

template <class T>
GenList<T>::GenList(char *Name)
{
	head= new GenListNode<T>();
	head->next=NULL;
	head->child=NULL;
	head->type=HEAD;
	head->element=NULL;
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
	current->next=NULL;
	current->child=NULL;
	current->element=element;
	current->mark=0;
	temp->next=current;



}

template <class T>
void GenList<T>::Insert(GenList<T> *genlist)
{
	GenListNode<T> *temp=current;
	GenListNode<T> *n=genlist->GetHead();
	n->mark=0;
	n->headNode.ref++;
   	current=new GenListNode<T>();
	current->child=n;
	current->next=NULL;
	current->type=LIST;
	temp->next=current;
}

template <class T>
void GenList<T>::Insert(GenListNode<T> *node)
{
	GenListNode<T> *temp=current;
	node->mark=0;
	node->headNode.ref++;
   	current=new GenListNode<T>();
	current->child=node;
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
void GenList<T>::MoveFirst()
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

    head->ViewAll();

}

template <class T>
bool GenList<T>::Insert(char *ListName, char*NodeName,char*ErrorMsg)
{//在NodeName中插入ListName链表
	if(strcmp(NodeName,(head->headNode).Name)==0)
	{//在最外层表中加入一个链表
		if(strcmp(ListName,(head->headNode).Name)==0)
		{//加入的链表就是自己
			Insert(this);
		}
		else
		{
			GenListNode<T>* tobeInsert=head->SearchInsert(ListName,head);
			if(tobeInsert==NULL)
			{//新的链表
					tobeInsert=new GenListNode<T>();
			    	tobeInsert->type=HEAD;
			    	tobeInsert->headNode.Name=ListName;
			    	tobeInsert->headNode.ref++;
			    	tobeInsert->next=NULL;
			    	tobeInsert->child=NULL;
			    	tobeInsert->mark=0;
					Insert(tobeInsert);
			}
			else
			{//已有的链表
			    	Insert(tobeInsert);
			}
		}
	}
	else
	{
		GenListNode<T>* WhichtoInsert=head->SearchInsert(NodeName,head);
		if(WhichtoInsert==NULL)
			return false;

		if(strcmp(ListName,(head->headNode).Name)==0)
		{//加入的链表就是自己
			WhichtoInsert->Insert(head,current);
		}
		else
		{
			GenListNode<T>* tobeInsert=head->SearchInsert(ListName,head);
			if(tobeInsert==NULL)
			{//新的链表
				tobeInsert=new GenListNode<T>();
				tobeInsert->type=HEAD;
				tobeInsert->headNode.Name=ListName;
				tobeInsert->headNode.ref++;
				tobeInsert->next=NULL;
				tobeInsert->child=NULL;
				tobeInsert->mark=0;
				Insert(tobeInsert);
			}
			else
			{//已有的链表
			    WhichtoInsert->Insert(tobeInsert,current);
			}
		}
	}
	return true;
}


template <class T>
bool GenList<T>::Insert(T element, char*NodeName)
{//在NodeName中插入element
	if(strcmp(NodeName,(head->headNode).Name)==0)
	{//在最外层表中加入
		Insert(element);
	}
	else
	{
		GenListNode<T>* WhichtoInsert=head->SearchInsert(NodeName,head);
		if(WhichtoInsert==NULL)
			return false;
		WhichtoInsert->Insert(element,current);
	}
	return true;
}

template <class T>
void GenList<T>::DeleteList(char *LName)
{
	head->Delete(LName);
}

	
