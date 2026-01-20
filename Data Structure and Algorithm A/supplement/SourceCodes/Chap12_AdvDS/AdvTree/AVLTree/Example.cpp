#include <iostream>
#include <iomanip>
#include "AVLTree.h"
using namespace std;

int main()
{
	char select;
	cout<<"选择avl树结点的类型: i:整数 c:单字符 f:浮点数 q:退出程序"<<endl;
	cin>>select;
	for(;(select!='i'&&select!='c'&&select!='f'&&select!='q');)//排除输入错误
	{
		cout<<"输入有错误"<<endl;
		cin>>select;
	}
	switch (select)
	{	case 'i':
		{
			avlTree<int> treeint;
			int val;char temp;
			cout<<"建立树:输入整形结点值，以-1000为结尾"<<endl;
			cin>>val;
			for(;val!=-1000;)
			{
				treeint.add(val);
				cin>>val;
			}
			cout<<"选择功能: a:增加结点，d:删除结点，f:查找结点，n:删除本树全部结点，v:显示当前树，q:退出程序"<<endl;
			do{
				cin>>select;
				for(;(select!='a'&&select!='d'&&select!='f'&&select!='n'&&select!='v'&&select!='q');)
				{
					cout<<"输入有错误"<<endl;
					cin>>select;
				}
				switch (select)
				{
					case 'a':cout<<"输入插入值:";cin>>val;treeint.add(val);break;
					case 'd':cout<<"输入删除值:";cin>>val;treeint.remove(val);break;
					case 'f':cout<<"输入查找值:";cin>>val;treeint.display(treeint.findValue(val));break;
					case 'n':cout<<"真的删除全部么(y/n)?";cin>>temp;if(temp=='y') treeint.deleteAllValue();else break;break;
					case 'v':treeint.display();break;
					case 'q':cout<<"Bye";return 0;
				}
				cout<<"选择功能: a:增加结点，d:删除结点，f:查找结点，n:删除本树全部结点，v:显示当前树，q:退出程序"<<endl;
			}while (select!='q');
			return 0;
		}
		case 'c':
		{
			avlTree<char> treeint;
			char val;char temp;
			cout<<"建立树:输入单个字符结点值，以 ~ 为结尾"<<endl;
			cin>>val;
			for(;val!='~';)
			{
				treeint.add(val);
				cin>>val;
			}
			cout<<"选择功能: a:增加结点，d:删除结点，f:查找结点，n:删除本树全部结点，v:显示当前树，q:退出程序"<<endl;
			do{
				cin>>select;
				for(;(select!='a'&&select!='d'&&select!='f'&&select!='n'&&select!='v'&&select!='q');)
				{
					cout<<"输入有错误"<<endl;
					cin>>select;
				}
				switch (select)
				{
					case 'a':cout<<"输入插入值:";cin>>val;treeint.add(val);break;
					case 'd':cout<<"输入删除值:";cin>>val;treeint.remove(val);break;
					case 'f':cout<<"输入查找值:";cin>>val;treeint.display(treeint.findValue(val));break;
					case 'n':cout<<"真的删除全部么(y/n)?";cin>>temp;if(temp=='y') treeint.deleteAllValue();else break;break;
					case 'v':treeint.display();break;
					case 'q':cout<<"Bye";return 0;
				}
				cout<<"选择功能: a:增加结点，d:删除结点，f:查找结点，n:删除本树全部结点，v:显示当前树，q:退出程序"<<endl;
			}while (select!='q');
			return 0;
		}
		case 'f':
		{
			avlTree<float> treeint;
			float val;char temp;
			cout<<"建立树:输入整形结点值，以-1000为结尾"<<endl;
			cin>>val;
			for(;val!=-1000;)
			{
				treeint.add(val);
				cin>>val;
			}
			cout<<"选择功能: a:增加结点，d:删除结点，f:查找结点，n:删除本树全部结点，v:显示当前树，q:退出程序"<<endl;
			do{
				cin>>select;
				for(;(select!='a'&&select!='d'&&select!='f'&&select!='n'&&select!='v'&&select!='q');)
				{
					cout<<"输入有错误"<<endl;
					cin>>select;
				}
				switch (select)
				{
					case 'a':cout<<"输入插入值:";cin>>val;treeint.add(val);break;
					case 'd':cout<<"输入删除值:";cin>>val;treeint.remove(val);break;
					case 'f':cout<<"输入查找值:";cin>>val;treeint.display(treeint.findValue(val));break;
					case 'n':cout<<"真的删除全部么(y/n)?";cin>>temp;if(temp=='y') treeint.deleteAllValue();else break;break;
					case 'v':treeint.display();break;
					case 'q':cout<<"Bye";return 0;
				}
				cout<<"选择功能: a:增加结点，d:删除结点，f:查找结点，n:删除本树全部结点，v:显示当前树，q:退出程序"<<endl;
			}while (select!='q');
			return 0;
		}
		case 'q':cout<<"Bye."<<endl;return 0;
	}
	return 0;
}
