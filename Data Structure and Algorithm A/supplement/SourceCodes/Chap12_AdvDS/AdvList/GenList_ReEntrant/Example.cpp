//使用广义表类的例子
//

#include "GenFunction.h"
#include <string.h>
int main()
{



// 构造表 (L1:(L2:(a, L1)), Lx:(L2, L3:b), Ly:(L3,c,d), L4:(L4))
	GenList<char*> *List=new GenList<char*>("List");
	cout<<"默认的表生成，表名:List 内容: List:()"<<endl;
	int operation;
	while(1)
	{
	
		operation=-1;
		cout<<"请选择你的操作 0:插入 1:删除 2:查看表内容 其他:退出"<<endl;
		cin>>operation;
		if((operation!=0)&&(operation!=1)&&(operation!=2))
			return 0;
		switch(operation)
		{
		case 0:
			{
				int type=-1;
				char*name=new char[10];
				cout<<"请输入要插入数据的表名:"<<endl;
				cin>>name;
				cout<<"请选择你要插入的种类 0:元素 1:子表"<<endl;
				cin>>type;
			
				if(type==0)
				{
					char* element=new char[10];
					cout<<"请输入元素值"<<endl;
					cin>>element;
					bool flag=List->Insert(element,name);
					if(!flag)
						cout<<"插入失败，请检查是否有该子表"<<endl;
				}
				else
				{
					char* newlist=new char[10];
					cout<<"请输入插入的子表名"<<endl;
					cin>>newlist;
					char* error=new char[10];
					bool flag=List->Insert(newlist,name,error);
					if(!flag)
						cout<<"插入失败，请检查待子表是否存在"<<endl;
				
				}
				delete []name;
				break;
			}
		case 1:
			{
				char * name=new char[10];
				cout<<"请输入待删除的子表名称"<<endl;
				cin>>name;
				List->DeleteList(name);
				if(strcmp(name,"List")==0)
				{
					cout<<"最外层表被删除，程序退出"<<endl;
					delete []name;
					return 0;
				}
				break;
			}
		case 2:
			{
		
				List->View(); 
		
			}

		}
	}
  return 0;
}
