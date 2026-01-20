#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;


#include "naive.h"


int main(int argc, char *argv[])
{
    string t;
    string p;
    int *N; 
    
    cin >> t; 
    cin >> p;
    
    cout << "the text to be matched is :" << t;
    cout << endl;
     
                    
    cout <<  NaiveStrMatching(t, p) << endl;
    cin >> t;
    
}
