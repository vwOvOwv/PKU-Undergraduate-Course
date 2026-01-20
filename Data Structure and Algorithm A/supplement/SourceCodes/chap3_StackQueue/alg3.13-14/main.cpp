#include <cstdlib>
#include <iostream>

#include "arrQueue.h"

using namespace std;

int main(int argc, char *argv[])
{
  arrQueue<int> *tst;
  
  tst = new arrQueue<int>(10);
  
  
  int n,r ;
  cin >> n;
  while (n>0) {
        tst->enQueue(n);
        n--;
  }
  tst->print();
  
  cin >> n;
  tst->deQueue(&r);
  
  cout << "deleted is : " << r << endl;
  
  tst->getFront(&r);
  cout << "front is : " << r << endl;
  
  tst->print();
  
  
  cin >> n;
  
  
}
