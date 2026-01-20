#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;


#include "kmp.h"


int main(int argc, char *argv[])
{
    string t;
    string p;
    int *N; 
    
    cin >> t; 
    cin >> p;
    
    cout << "the text to be matched is :" << t;
    
     
    N = findNext(p);
    cout << "the next array of pattern" << p << "is :" << endl;
    for (int i=0; i< p.length(); i++)
        cout << N[i] << " " ;
    cout << endl;
                    
    cout <<  KMPStrMatching(t, p, N) << endl;
    cin >> t;
    
}
