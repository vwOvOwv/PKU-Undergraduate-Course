#include<iostream>
#include<string>

using namespace std;

void getNext(string p,int *next){
    int n=p.size();
    next[0]=-1;
    int j=0,k=0;
    while(j<n){
        while(k>=0&&p[k]!=p[j])
            k=next[k];
        k++,j++;
        if(j==n)
            break;
        if(p[k]==p[j])
            next[j]=next[k];
        else
            next[j]=k;
    }
}

int kmp(string p,string t){
    int plen=p.size(),tlen=t.size();
    if(plen>tlen)   
        return -1;
    int i=0,j=0;
    int *next=new int[plen];
    getNext(p,next);
    while(j<plen&&i<tlen){
        if(j==-1||t[i]==p[j])
            j++,i++;
        else
            j=next[j];
    }
    if(j>=plen)
        return (i-plen);
    return -1;
}

int main(){
    string p;
    cin>>p;
    string t;
    cin>>t;
    cout<<kmp(p,t);
    return 0;
}