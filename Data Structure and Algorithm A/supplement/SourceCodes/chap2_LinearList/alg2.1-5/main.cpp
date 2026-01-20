#include <cstdlib>
#include <iostream>

#include "arrList.h"
 
using namespace std;

int main(int argc, char *argv[])
{
  int val, p, n;
   
  arrList<int> a(10);
  
  cin >> n;
    
  for (int i=0; i <n; i++) { 
    cin >> p; 
    cin >> val;
    a.insert(p, val);
  }
  a.print();
  
  cin >> n; 
  cout << a.getPos(n) << endl;
   
  cin >> p;
  a.del(p);
  
  a.print();
  
   cin >> n;
   cout << a.getPos(n) << endl;
     
  cin >> n;
   
  for (int i=0; i <n; i++) { 
    cin >> p; 
    cin >> val;
    a.insert(p, val);
  }
 a.print();
  
  cin >> p;
  a.del(p);
  
  a.print();
  
   cin >> n;
   return 0;  
}
