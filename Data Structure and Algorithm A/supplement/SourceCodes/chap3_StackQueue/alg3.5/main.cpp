#include <cstdlib>
#include <iostream>
#include "calc.h"

using namespace std;

int main(int argc, char *argv[])
{
    int s;
    cout << "begin" << endl;
    

     Calculator *cal = new Calculator();
     cal->Run();
         
     cout << "end" << endl;
      cin >> s;
}
