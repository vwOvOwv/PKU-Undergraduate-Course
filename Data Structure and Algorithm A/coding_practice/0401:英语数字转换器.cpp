#include <iostream>
#include<map>
#include <string>
#include <string.h>
using namespace std;
map<string,int> dic;
int main(){
    dic["negative"]=-1;
    dic["zero"]=0;
    dic["one"]=1;
    dic["two"]=2;
    dic["three"]=3;
    dic["four"]=4;
    dic["five"]=5;
    dic["six"]=6;
    dic["seven"]=7;
    dic["eight"]=8;
    dic["nine"]=9;
    dic["ten"]=10;
    dic["eleven"]=11;
    dic["twelve"]=12;
    dic["thirteen"]=13;
    dic["fourteen"]=14;
    dic["fifteen"]=15;
    dic["sixteen"]=16;
    dic["seventeen"]=17;
    dic["eighteen"]=18;
    dic["nineteen"]=19;
    dic["twenty"]=20;
    dic["thirty"]=30;
    dic["forty"]=40;
    dic["fifty"]=50;
    dic["sixty"]=60;
    dic["seventy"]=70;
    dic["eighty"]=80;
    dic["ninety"]=90;
 
    dic["hundred"]=100;
    dic["thousand"]=1000;
    dic["million"]=1000000;
 
    string str;
    long long tmp=0,ans=0;
    while(cin>>str){
 
        if(dic[str]==-1){
            cout<<"-";
        }
        else if(dic[str]==100){
            tmp*=100;
        }
        else if(dic[str]==1000 || dic[str]==1000000){
            tmp*=dic[str];
            ans+=tmp;
            tmp=0;
        }
        else{
            tmp+=dic[str];
        }
 
        if(cin.peek()=='\n'){
            ans+=tmp;
            tmp=0;
            cout<<ans<<endl;
            ans=0;
        }
    }
 
    return 0;
}
