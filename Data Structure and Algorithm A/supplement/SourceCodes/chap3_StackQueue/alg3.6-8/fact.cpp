#include <cstdlib>
#include <iostream>

#include "arrStack.h" 

using namespace std;

long factorial(long n);

int main(int argc, char *argv[])
{
    long x, m;
	cin >> x;
    cout << "the factioral of "
         << x 
         << "is " ;
	m = factorial(x);
	cout << m 
       <<endl;
    cin >> x;
}


/* recurive version
long factorial(long n)  {
	if (n <= 0) 
		return 1;
	return n * factorial(n-1);
}


// iterated version 
long factorial(long n) {
		long  m = 1;
		long  i ;
		if (n > 0)
			for ( i = 1;  i <= n;  i++ )
				m = m * i ;
		return m ;
}    

*/

// nonrevursive with stack
long factorial(long n) {
     arrStack<long> s(20);
	 long tmp;
     
	 long m = 1;
	 while (n > 0)                  	// 不满足递归出口      
		s.push(n--);				// 按递归规则把相应数据压栈
	 while (!s.isEmpty()) {			// 递归出口满足，开始进行返回处理
		s.pop(&tmp);
		m *= tmp;
     } 
	 return m ;
}   

