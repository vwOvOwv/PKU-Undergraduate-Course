#include <cstdlib>
#include <iostream>

using namespace std;

#include <cstdlib>
#include <iostream>

#include "lnkstack.h" 

using namespace std;

long f(long n);

int main(int argc, char *argv[])
{
    Link<int>* test = new Link<int>(10, NULL);
    
    cout << test->data << endl;
    long x, m;
	cin >> x;
    
    
    cout << "the factioral of "
         << x 
         << "  is " ;
	m = f(x);
	cout << m 
       <<endl;
    cin >> x;
}


// nonrevursive with stack
long f(long n) {
     lnkStack<long> s(0);
    
	 long tmp;
     
	 long m = 1;
	 while (n > 1) {                  	// 不满足递归出口      
		s.push(n);
        n--;
     }				// 按递归规则把相应数据压栈
	 while(s.pop(&tmp)) 		// 递归出口满足，开始进行返回处理
		m *= tmp;
	 return m ;
}   
