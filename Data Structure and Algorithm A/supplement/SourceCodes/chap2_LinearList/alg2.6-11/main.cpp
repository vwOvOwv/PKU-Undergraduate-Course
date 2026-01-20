#include <cstdlib>
#include <iostream>

using namespace std;

#include <iostream>

#include "lnkList.h"

using namespace std;

int main(int argc, char *argv[])
{
  lnkList<int> *lk;
  
  lk = new lnkList<int>(0);

  lk->print();
  int n, p, val;
  
  
  cin >> n;
  
  while (n > 0) {
        cin >> p;
        cin >> val;
        lk->insert(p, val);
        n--;
}
  lk->print();
  
  cin >> p;
  
  lk->del(p);
  
  lk->print();

  cin >> p;
}
