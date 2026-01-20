#include<iostream>
#include<string>

using namespace std;

int main(){
    string str1,str2;
    while(cin>>str1>>str2){
        int l1=str1.size(),l2=str2.size();
        if(l1>l2){
            cout<<"No"<<endl;
            continue;
        }
        int index1=0,index2=-1;
        while(index1<l1){
            int pos=str2.substr(index2+1).find(str1[index1])+index2+1;
            // cout<<"pos="<<pos<<endl;
            if(pos!=l2&&pos>index2){
                index2=pos;
                index1++;
            }
            else{
                cout<<"No"<<endl;
                goto loop;
            }
        }
        cout<<"Yes"<<endl;
        loop:;
    }
}