#include <cstdlib>

#include <iostream>
#include "myString.h"

using namespace std;

int main(int argc, char *argv[])
{
   char *s = "hello friend!";
   
   String* t = new String(s);
   
   cout << t->Substr(2,6) << endl;
       
   
    system("PAUSE");
    return EXIT_SUCCESS;
}
