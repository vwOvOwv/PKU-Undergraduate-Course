

template <class T>
class MaxHeap  
{
private:
	T* heapArray;							//存放堆数据的数组
	int CurrentSize;						//当前堆中元素数目
	int MaxSize;							//堆所能容纳的最大元素数目
public:
	MaxHeap(T* array,int num,int max);
	virtual ~MaxHeap(){};					//析构函数
	void BuildHeap();
	bool isLeaf(int pos) const;				//如果是叶结点，返回TRUE
	int leftchild(int pos) const;			//返回左孩子位置
	int rightchild(int pos) const;			//返回右孩子位置
	int parent(int pos) const;				//返回父结点位置
	bool Remove(int pos, T& node);			//删除给定下标的元素
	void SiftDown(int left);				//筛选法函数，参数left表示开始处理的数组下标
	void SiftUp(int position);			 	//从position向上开始调整，使序列成为堆
	bool Insert(const T& newNode);			//向堆中插入新元素newNode
	void MoveMax();							//从堆顶移动最大值到尾部
	T& RemoveMax();							//从堆顶删除最大值
};

template<class T>
MaxHeap<T>::MaxHeap(T* array,int num,int max)
{
	heapArray=array;
	CurrentSize=num;
	MaxSize=max;
	BuildHeap();
}

template<class T>
void MaxHeap<T>::BuildHeap()
{
	for (int i=CurrentSize/2-1; i>=0; i--) 
		SiftDown(i); 
}

template<class T>
bool MaxHeap<T>::isLeaf(int pos) const
{
		return (pos>=CurrentSize/2)&&(pos<CurrentSize);
}

template<class T>
int MaxHeap<T>::leftchild(int pos) const
{
	return 2*pos+1;						//返回左孩子位置
}

template<class T>
int MaxHeap<T>::rightchild(int pos) const
{
	return 2*pos+2;						//返回右孩子位置
}

template<class T>
int MaxHeap<T>::parent(int pos) const	//返回父节点位置
{
	return (pos-1)/2;
}

template<class T>
void MaxHeap<T>::SiftDown(int left)
{
	//准备
	int i=left;							//标识父结点
	int j=2*i+1;						//标识关键值较小的子结点		
	T	temp=heapArray[i];				//保存父结点
	//过筛
	while(j<CurrentSize)
	{
		if((j<CurrentSize)&&(heapArray[j]<heapArray[j+1]))
			j++;						//j指向右子结点
		if(temp<heapArray[j])
		{
			heapArray[i]=heapArray[j];
			i=j;
			j=2*j+1;					//向下继续
		}
		else break;
	}
	heapArray[i]=temp;
}

template<class T>
void MaxHeap<T>::SiftUp(int position) 
{//从position向上开始调整，使序列成为堆
	int temppos=position;
	T temp=heapArray[temppos];
	int parentpos=0;
	if(temppos>0)
		parentpos=parent(temppos);
	while((temppos>0)&&(heapArray[parentpos]<temp))
	{
		heapArray[temppos]=heapArray[parentpos];
		temppos=parentpos;
		parentpos=parent(temppos);
	}
	heapArray[temppos]=temp;
}

template<class T>
bool MaxHeap<T>::Insert(const T& newNode)
{//向堆中插入一个结点
	if(CurrentSize==MaxSize)				//堆空间已经满
		return false;
	heapArray[CurrentSize]=newNode;
	SiftUp(CurrentSize);					//向上调整
	CurrentSize++;
}

template<class T>
T& MaxHeap<T>::RemoveMax()
{
	if(CurrentSize==0)
	{
		//AfxMessageBox("Can't Delete");

	}
	else
	{
		T temp=heapArray[0];					//取堆顶元素
		heapArray[0]=heapArray[CurrentSize-1];	//堆末元素上升至堆顶
		CurrentSize--;
		SiftDown(0);							//从堆顶开始筛选
		return temp;
	}
}

template<class T>
void MaxHeap<T>::MoveMax()
{
	if(CurrentSize==0)
	{
		//堆为空
		return;
	}
	else
	{
		T temp=heapArray[0];					//取堆顶元素
		heapArray[0]=heapArray[CurrentSize-1];	//堆末元素上升至堆顶
		CurrentSize--;
		SiftDown(0);							//从堆顶开始筛选
		heapArray[CurrentSize]=temp;
	}
	return;
}

template<class T>
bool MaxHeap<T>::Remove(int pos, T& node)
{// 删除给定下标的元素
	if((pos<0)||(pos>CurrentSize))
		return false;
	heapArray[pos]=heapArray[--CurrentSize];	//指定元素置于最后
	SiftUp(pos);								//上升筛
	SiftDown(pos);								//向下筛	
	node=heapArray[CurrentSize];
	return true;
}

