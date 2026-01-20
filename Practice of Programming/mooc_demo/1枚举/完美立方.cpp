#include<iostream>
using namespace std;

int a,b,c,d;
int n;
int main()
{
	cin>>n;
	for(a=2;a<=n;a++)
		for(b=2;b<=n;b++)
			for(c=b;c<=n;c++)
				for(d=c;d<=n;d++)
					if(a*a*a==b*b*b+c*c*c+d*d*d)
						printf("Cube = %d, Triple = (%d,%d,%d)\n",a,b,c,d);
	 return 0;
}
