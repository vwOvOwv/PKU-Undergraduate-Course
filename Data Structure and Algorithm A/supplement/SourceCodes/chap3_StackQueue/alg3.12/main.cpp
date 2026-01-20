#include <cstdlib>
#include <iostream>

#include "lnkQueue.h"

using namespace std;

int main(int argc, char *argv[])
{
    lnkQueue<int> *tst = new lnkQueue<int>(0);
    int n;
    
    cin >> n;
    for (int i=0; i<n;i++) 
      tst->enQueue(i);
    tst->print();
    cin >> n;
    
   
    tst->deQueue(&n);
    cout << "the deleted front is:" << n << endl;
    cin >>n;
    
     tst->getFront(&n);
    cout << "the front is:" << n << endl;
    
    cin >> n;
    
    
}
