//this is avl tree node 
using namespace std;


template <class T> class avlNode//平衡二叉树结点类
{
	public:
		avlNode(T val);//构造函数
		avlNode(T val,avlNode<T> *left,avlNode<T> *right,int bf);
		avlNode<T>* copy()const;//复制以当前结点为根的二叉树 ：(写多了，没用上)                           
		void release();//删除以当前结点为根的左右子树									
		void left(avlNode *);//把当前结点的左指针修改为函数的参数
		avlNode<T>* left()const;//左子结点访问，返回左结点的指针
		void right(avlNode *v);//把当前结点的右指针修改为函数的参数
		avlNode<T>* right()const;//右子结点访问，返回右结点的指针
		int add(avlNode<T>* &p,T val);//插入一个值；返回新的avl树的根结点的指针
		void preorderview(avlNode<T> *current,int i=-1);//前序周游
		avlNode<T>* remove(T val,avlNode<T>* &waste,int &flag);//删除以当前结点的为根的avl树中的val结点
		avlNode<T>* findNodeValue(T val);//查找val结点
		T value;//码值
	private:
		int bf;// balance factor
		avlNode<T>* leftptr;//左右指针
		avlNode<T>* rightptr;
		//avlNode<T>* restoreLeftBalance(int oldbf);//删除时左子树失衡的时候调整，返回新的树根的指针
		//avlNode<T>* restoreRightBalance(int oldbf);//删除时右子树失衡的时候调整，返回新的树根的指针
		avlNode<T>* removeLeftmostElement(avlNode<T>* &childptr,int &flag);//找到最左的结点
		//avlNode<T>* removeBalanceLeft();//从左子树删除的时候判断层数的改变
		//avlNode<T>* removeBalanceRight();//从右子树删除的时候判断层数的改变
		avlNode<T>* LL_singleRotation();//在插入时候左子树LL失衡的时候调整，返回新的树根的指针
		avlNode<T>* RR_singleRotation();//在插入时候右子树RR失衡的时候调整，返回新的树根的指针
		avlNode<T>* LR_doubleRotation();//在插入时候左子树LR失衡的时候调整，返回新的树根的指针
		avlNode<T>* RL_doubleRotation();//在插入时候右子树RL失衡的时候调整，返回新的树根的指针
	
};


template <class T> avlNode<T>::avlNode(T val)
{
	value=val;
	leftptr=NULL;
	rightptr=NULL;
	bf=0;
}
template <class T> avlNode<T>::avlNode(T val,avlNode<T> *left,avlNode<T> *right,int bf=0)
{
	value=val;
	leftptr=left;
	rightptr=right;
	bf=bf;
}
template <class T> avlNode<T>* avlNode<T>::copy()const
{
	avlNode<T> *nl,*nr;
	nl=(leftptr==NULL?NULL:leftptr->copy());//递归调用复制左子树
	nr=(rightptr==NULL?NULL:rightptr->copy());//递归调用复制右子树
	avlNode<T>* node=new avlNode<T>(value,nl,nr,bf);
	if(node!=NULL)   //?? ==NULL
		cout<<"error"<<endl;
	else
		return node;
}
template <class T> void avlNode<T>::release()
{
	if (leftptr)
	{//删除左子树中的结点
		leftptr->release();//递归调用
		delete leftptr;
		leftptr=0;
	}
	if (rightptr)
	{//删除右子树中的结点
		rightptr->release();//递归调用
		delete rightptr;
		rightptr=0;
	}
}
template <class T> avlNode<T>* avlNode<T>::left()const
{
	return leftptr;
}
template <class T> void avlNode<T>::left(avlNode<T>* v)
{
	leftptr=v;
}
template <class T> avlNode<T>* avlNode<T>::right()const
{
	return rightptr;
}
template <class T> void avlNode<T>::right(avlNode<T>* v)
{
	rightptr=v;
}
template <class T> int avlNode<T>::add(avlNode<T>* &rp,T val)
{//返回值表明以当前结点为根的树是否再插入之后增高，0:非增高，非0:增高
	if (val<value)
	{//左子树插入
		if (rp->left()==NULL)
			rp->left(new avlNode<T>(val));
		else if(rp->left()->add(rp->leftptr,val)==0)//插入后子树没有增高
			return 0;
		if (rp->bf==-1)
		{//原来已经倾斜，左边失衡，需要做平衡处理
	       if (rp->left()->bf<0)  //插入在左侧，单旋转
	             rp = LL_singleRotation();
	       else rp = LR_doubleRotation();	//插入在右侧，双旋转
		   return 0;
		}
		return --bf;   // bf=(0, +1)的情况，不需要调整树，只要修改bf
	}
	else
	{
		if (rp->right()==NULL)
			rp->right(new avlNode<T>(val));
		else if (rp->right()->add(rp->rightptr,val)==0)//插入后子树没有增高 
			return 0;
		if(rp->bf==1)
		{//原来已经倾斜，需要做平衡处理
	       if (rp->right()->bf>0)    //插入在右侧，单旋转
	           rp = RR_singleRotation();
	       else rp = RL_doubleRotation();  //插入点在右侧.双旋转
		   return 0;
		}
		return ++bf; // bf=(0, -1)的情况，不需要调整树，只要修改bf
	}
}


template <class T> avlNode<T>* avlNode<T>::remove(T val,avlNode<T>* &waste,int &flag)
{
	if (val==value)
	{
		waste=this;
		//当没有右子树的时候返回左子树
		if(right()==NULL)
		{
			flag=1;
			return left();
		}
		//删除右子树中的最小结点
		int oldbf=right()->bf;
		avlNode* newroot;
		right(right()->removeLeftmostElement(newroot,flag));//找到后返回已经平衡的avl树的根指针
		newroot->left(left());
		newroot->right(right());
		if((flag==1)&&(bf==1))
			flag=1;
		else flag=0;
		if(flag==1)
		{
			newroot->bf=bf--;
		}
		else newroot->bf=bf;
        //左树的平衡
		avlNode<T>* rightchild=newroot->right();
	    if (rightchild==NULL) 
		    bf--;
	    else if((rightchild->bf!=oldbf)&&(rightchild->bf==0))
		    bf--;
    	if (bf<-1)
		{
		    int newoldbf=newroot->left()->bf;
	        if (newoldbf>0)
			{//双旋转
    	       return newroot->LR_doubleRotation();
			}
	        else
			{//单旋转
		       return newroot->LL_singleRotation();
           	}
		}
	    return newroot;
		
	}
	else if(val<value)
	{//从左子树中删除
		if(left()==NULL)
			return this;
		//执行删除
		int oldbf=left()->bf;
		left(left()->remove(val,waste,flag));//递归调用
        //调整左子树
        avlNode<T>* leftchild=left();
	//	if(flag==1)
	//		bf++;
	    //计算删除后的子树对当前的根结点的平衡因子的影响
	    if (leftchild==NULL)
		   bf++;
	    else if((leftchild->bf!=oldbf)&&(leftchild->bf==0))
		   bf++;
	    if (bf>1)//失衡
		{//调整
		   int newoldbf=right()->bf;
	       if (newoldbf<0)//双旋转
		   {
	           return RL_doubleRotation();
		   }
	       else
		   {//单旋转
		       avlNode* temp= RR_singleRotation();
			   if(flag==1)
				   bf++;
			   return temp;
		   }
		}
    	return this;
	}
	else
	{//从右子树中删除
		if(right()==NULL)
			return this;
		//执行删除
		int oldbf=right()->bf;
		right(right()->remove(val,waste,flag));//递归调用
		//调整右子树
		avlNode<T>* rightchild=right();
    	if (rightchild==NULL) 
		  bf--;
	    else if((rightchild->bf!=oldbf)&&(rightchild->bf==0))
		  bf--;
	    if (bf<-1)
		{
		  int newoldbf=left()->bf;
	      if (newoldbf>0)
		  {//双旋转
    	      return LR_doubleRotation();
		  }
	      else
		  {//单旋转
			  avlNode* temp= LL_singleRotation();
			  if(flag==1)
			    bf--;
			   return temp;
		   }
		} 
	    return this;
	}
}

template <class T> avlNode<T>* avlNode<T>::removeLeftmostElement(avlNode<T>* &childptr,int &flag)
{//flag 表示子树高度是否变化
	avlNode* leftchild=left();
	//找到最小的值，返回，否则递归调用
	if (leftchild==NULL)
	{
		childptr=this;
		flag=1;
		return right();
	}
	int oldbf=leftchild->bf;
	left(leftchild->removeLeftmostElement(childptr,flag));//递归调用
	//调整左子树平衡
	avlNode<T>* newleftchild=left();
	//计算删除后的子树的高度变化
	if((newleftchild==NULL)&&(right()==NULL))
		flag=1;
	//计算删除后的子树对当前的根结点的平衡因子的影响
	if (newleftchild==NULL)
		bf++;
	else if((newleftchild->bf!=oldbf)&&(newleftchild->bf==0))
		bf++;
	if (bf>1)//失衡
	{//调整
		int newoldbf=right()->bf;
	    if (newoldbf<0)//双旋转
		{
	       return RL_doubleRotation();
		}
	    else
		{//单旋转
		   return RR_singleRotation();
		}
	}
	return this;
}


template <class T> avlNode<T>* avlNode<T>::findNodeValue(T val)
{
	if (val==value)
	{
		return this;
	}
	else if (val>value)
	{//大于的话在右子树中查找
		if (right()!=NULL)
			return right()->findNodeValue(val);//递归调用
		else 
			return NULL;
	}
	else
	{//小于的话在左子树中查找
		if (left()!=NULL)			
			return left()->findNodeValue(val);//递归调用
		else
			return NULL;
	}
}
template <class T> void avlNode<T>::preorderview(avlNode<T> *current,int i)
{
	i++;//层计数器
	if (current)
	{
		cout<<setw(8)<<current->value<<setw(10)<<current->bf<<" "<<setw(6)<<i<<endl;
		preorderview(current->left(),i);//递归调用
		preorderview(current->right(),i);//递归调用
	}
}

template <class T> avlNode<T>* avlNode<T>::LL_singleRotation()
{
	avlNode<T> *p;
	p=left();
	left(p->right());
	bf=0;
	p->right(this);
    if(p->bf==0)
	  p->bf=1;
	else p->bf=0;
	return p;
}

template <class T> avlNode<T>* avlNode<T>::LR_doubleRotation()
{
	avlNode<T> *p,*q;
	q=left();
	p=q->right();
	q->right(p->left());
	left(p->right());
	p->left(q);
	bf=q->bf=0;
	if(p->bf==-1) bf=1;
	if(p->bf==1) q->bf=-1;
	p->right(this);
	p->bf=0;
	return p;
}

template <class T> avlNode<T>* avlNode<T>::RR_singleRotation()
{
	avlNode<T> *p;
	p=right();
	right(p->left());
	bf=0;
	p->left(this);
    if(p->bf==0) 
	  p->bf=-1;
	else p->bf=0;
	return p;
}

template <class T> avlNode<T>* avlNode<T>::RL_doubleRotation()
{
	avlNode<T> *p,*q;
	q=right();
	p=q->left();
	q->left(p->right());
	right(p->left());
	p->right(q);
	bf=q->bf=0;
	if(p->bf==-1) q->bf=1;
	if(p->bf==1) bf=-1;
	p->left(this);
	p->bf=0;
	return p;
}

