//使用十字链表实现稀疏矩阵
#include <iostream>
#include <afxtempl.h>
using namespace std;

class Element //(行、列、元素)
{
public:
	int row;
	int col;
	int data;
};

//首先定义节点类.T是节点存储的数据类型
//如果是头结点则不存储任何数值
template<class T> class SMatrix;

template<class T>
class OLNode
{
	friend class SMatrix<T>;
    private:
		int row,col;//矩阵的行和列
		T element;//矩阵中存储的数据
        OLNode<T>* right,*down;//指向下一个节点的指针
	public:
		OLNode(){right=NULL;down=NULL;};
};

//稀疏矩阵
template<class T>
class SMatrix
{
	private:
		int rownum,colnum;//行列数
		void CreateMatrix(CArray<Element,Element>&Data);//only for local use 
	public:
		OLNode<T> **rowhead,**colhead;//矩阵的第一个非零元素
		SMatrix();
		void MallocMem(int row,int col); //为矩阵收集内存
		int GetRownum();
		int GetColnum();
		void CreateMatrix();//建立矩阵
        void PrintAll();
		void FreeMem();
		SMatrix<int> * MatrixMutil(SMatrix<int> *left,SMatrix<int>*right);
};

template<class T>
SMatrix<T>::SMatrix()
{
	rowhead=colhead=NULL;
}

template<class T>
int SMatrix<T>::GetRownum()
{
	return rownum;
}

template<class T>
int SMatrix<T>::GetColnum()
{
	return colnum;
}

template<class T>
void SMatrix<T>::MallocMem(int row,int col)
{
	rownum=row;
	colnum=col;
	rowhead=new OLNode<T>*[row+1];
    colhead=new OLNode<T>*[col+1];
    for(int count=0;count<=row;count++)
	{//初始化为头结点
		rowhead[count]=new OLNode<T>();
		rowhead[count]->row=-1;
		rowhead[count]->col=-1;
	}
	int count;
    for(count=0;count<=col;count++)
	{
        colhead[count]=new OLNode<T>();
		colhead[count]->row=-1;
        colhead[count]->col=-1;
	}
}

template<class T>
void SMatrix<T>::CreateMatrix()
{
	//首先
    if((rowhead!=NULL)||(colnum!=NULL))
	   FreeMem();
	cout<<"输入矩阵的行数:";
	cin>>rownum;
	cout<<endl;
	cout<<"输入矩阵的列数:";
	cin>>colnum;
    cout<<endl;
	//为行列指针收集空间
    MallocMem(rownum,colnum);
	

	cout<<"请输入矩阵的元素，格式<行，列，数据>+回车,结束输入请输入-1"<<endl;
    int i,j;
	T ele;
	while(1)
	{
		cin>>i;
        if(i==-1) break;
		cin>>j;
		cin>>ele;
		if((i>rownum)||(j>colnum)||(i<1)||(j<1))
		{
			cout<<"不正确的行，列号"<<endl;
			continue;
		}
		OLNode<T>* temp=new OLNode<T>();
		temp->row=i;
		temp->col=j;
		temp->element=ele;
		//处理行指针
		//查找位置 
		OLNode<T>* t2=rowhead[i];
        OLNode<T>* t1=NULL;
		while((t2!=NULL)&&(j>t2->col))
		{
			t1=t2;
			t2=t2->right;
		}
		if((t2!=NULL)&&(t2->col==j))
		{//修改值
			t2->element=temp->element;
		}
		else
		{
		 //插入新的指针
		    t1->right=temp;
		    temp->right=t2;
		}
		
		//处理列指针
		//查找位置 
		t2=colhead[j];
        t1=NULL;
		while((t2!=NULL)&&(i>t2->row))
		{
			t1=t2;
			t2=t2->down;
		}
		if((t2!=NULL)&&(t2->row==i))
		{//修改值
			t2->element=temp->element;
		}
		else
		{	 //插入新的指针
	        t1->down=temp;
	        temp->down=t2;
		}
	
	}//end of while
}



template<class T>
void SMatrix<T>::PrintAll()
{
	int i,j;
	for(i=1;i<=rownum;i++)
	{
		OLNode<T>*temp= rowhead[i]->right;
		for(j=1;j<=colnum;j++)
		{
			if((temp!=NULL)&&(temp->col==j))
			{
				cout<<" "<<temp->element<<" ";
				temp=temp->right;
			}
			else
				cout<<" 0 ";
		}
		cout<<endl;
	}
}
			
template<class T>
void SMatrix<T>::FreeMem()
{
	int i,j;
	for(i=1;i<=rownum;i++)
	{
		OLNode<T>*temp= rowhead[i];
		for(j=1;j<=colnum;j++)
		{
			if((temp!=NULL)&&(temp->col==j))
			{
				delete temp;
			}
		}
	}
	delete rowhead;
	delete colhead;
}

template <class T>
SMatrix<int>* SMatrix<T>::MatrixMutil(SMatrix<int>*left,SMatrix<int>*right)
{
   	if(left->GetColnum()!=right->GetRownum())
		return NULL;//行列不匹配不能相乘

	int I=0; //第一个矩阵的行数
	int J=0; //第二个矩阵的列数
    SMatrix<T> *ResultMatrix=new SMatrix<T>();
    ResultMatrix->MallocMem(left->GetRownum(),right->GetColnum());
	for(I=1;I<=left->GetRownum();I++)
	{
		OLNode<T>* RowNext=ResultMatrix->rowhead[I];

		for(J=1;J<=right->GetColnum();J++)
		{
			OLNode<T>* ColNext=ResultMatrix->colhead[J];
			int result=0;
			OLNode<int> * rows=left->rowhead[I]->right;
			OLNode<int>* cols=right->colhead[J]->down;
			if((rows==NULL)||(cols==NULL))
				break;//新行没有非零元素
            
			while((rows!=NULL)&&(cols!=NULL))
			{
				if(rows->col<cols->row)
				{
					rows=rows->right;
				}
				else
					if(rows->col>cols->row)
					{
						cols=cols->down;
					}
					else
					{//都有元素可以相乘
						result=result+cols->element*rows->element;
						cols=cols->down;
                        rows=rows->right;
					}
			}
			if(result==0) continue;
			//插入到结果矩阵中
			OLNode<T>* temp=new OLNode<T>();
		    temp->row=I;
		    temp->col=J;
		    temp->element=result;
		
			//加入一个新的元素到下一个位置
			RowNext->right=temp;
			RowNext=RowNext->right;
		    //调转到合适的位置
			while(ColNext->down!=NULL)
				ColNext=ColNext->down;
			ColNext->down=temp;
			ColNext=ColNext->down;
			
		}//计算出一个元素，放入ResultMatrix
	
	}
	/////////////////乘法做完，生成新的矩阵
    return ResultMatrix;
}


    


