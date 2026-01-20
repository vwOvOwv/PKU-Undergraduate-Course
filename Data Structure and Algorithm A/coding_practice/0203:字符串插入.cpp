#include<iostream>
#include<string>

using namespace std;

int main(){
    string s1,s2;
    while(cin>>s1>>s2){
    int l1=s1.length();
    char maxi=s1[0];
    int maxpos=0;
    for(int i=1;i<l1;i++){
        if(s1[i]>maxi){
            maxi=s1[i];
            maxpos=i;
        }
    }
    s1.insert(maxpos+1,s2);//前插入
    cout<<s1<<endl;
    }
    return 0;
}