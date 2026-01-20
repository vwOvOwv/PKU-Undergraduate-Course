#include <cstdlib>
#include <iostream>
#include <string.h>


using namespace std;

int main(int argc, char *argv[])
{
    char s[20], t[20];
    
    cin >> s;
    cin >> t;
    cout << strcmp(s,t) << endl;
    cin >> s;
    system("PAUSE");
    return EXIT_SUCCESS;
}

// compare two strings,and return 1 if s1> s2, and -1 if s1< s2
int strcmp(char *s1, char *s2) {
    int i = 0;
    while (s2[i] != '\0'  ||  s1[i] != '\0' ) {
        if (s1[i] > s2[i])
           return 1;
        else if (s1[i] < s2[i])
             return -1;
        i++;
    }
    if (s1[i] == '\0' && s2[i] != '\0')
       return -1;
    else if (
    s2[i] == '\0' && s1[i] != '\0')
         return 1;
    return 0;
}
