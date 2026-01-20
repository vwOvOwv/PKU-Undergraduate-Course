#include "SMatrix.h"

int main()
{
	SMatrix<int> m1,m2;
	m1.CreateMatrix();
    m1.PrintAll();
	m2.CreateMatrix();
	m2.PrintAll();
    SMatrix<int> *m=m1.MatrixMutil(&m1,&m2);
	if(m!=NULL)
	{
		cout<<"矩阵相乘的结果为:"<<endl;
		m->PrintAll();
	}
	return 0;
}