//使用广义表类的例子
//

#include "GenFunction.h"

int main()
{
/*

	//构造表((a1,a2),(a1,a2))
	GenList<char*> *List=new GenList<char*>("List");
	GenList<char*> *List22=new GenList<char*>("L22");
	List22->Insert("a1");
	List22->Insert("a2");
	List->Insert(List22);
	List->Insert(List22);
	List->View(); 
*/


// 构造表 (L1:(L2:(a, L1), Lx:(L2, L3:b), Ly:(L3,c,d), L4:(L4))
	GenList<char*> *List=new GenList<char*>("List");
	GenList<char*> *List1=new GenList<char*>("L1");
    GenList<char*> *List2=new GenList<char*>("L2");
	GenList<char*> *List3=new GenList<char*>("L3");
	GenList<char*> *List4=new GenList<char*>("L4");
//	GenList<char*> *Listx=new GenList<char*>("Lx");
//	GenList<char*> *Listy=new GenList<char*>("Ly");
	GenList<char*> *Listx=new GenList<char*>("");
	GenList<char*> *Listy=new GenList<char*>("");
	List3->Insert("b");
	List4->Insert(List4);
	Listy->Insert(List3);
	Listy->Insert("c");
	Listy->Insert("d");
	List2->Insert("a");
	List2->Insert(List1);
	List1->Insert(List2);
    Listx->Insert(List2);
	Listx->Insert(List3);
	List->Insert(List1);
	List->Insert(Listx);
	List->Insert(Listy);
	List->Insert(List4);

	List->ViewList(); 
    return 0;
}