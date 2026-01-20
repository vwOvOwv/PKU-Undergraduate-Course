#include<iostream>
#include<vector>
using namespace std;


string s;
void get_next1(vector<int>&next){
    int n=s.size();
    next[0]=-1;
    int j=0,k=-1;
    while(j<=n){
        while(k>=0&&s[j]!=s[k])
            k=next[k];
        j++,k++;
        if(j==n+1)
            break;
        next[j]=k;
    }
}
void get_next2(vector<int>&next){//这里next[j]的含义是包含s[j]的最长首尾匹配真子串的长度
    int n=s.size();
    next[0]=0;
    int j=0,k=-1;
    while(j<n){
        while(k>0&&s[j+1]!=s[k])//包含s[j]，故要比较s[j+1]和s[k]
            k=next[k-1];//失配后的处理规则
        if(k==0&&s[j+1]!=s[k])//没有找到包含s[j]的最长首尾匹配真子串
            k=-1;
        j++,k++;
        if(j==n)
            break;
        next[j]=k;
    }
}
int main(){
    s="abcdabcaab";
    vector<int>next1(11,0),next2(10,0);
    get_next1(next1);
    get_next2(next2);
    cout<<s<<endl;
    for(auto num:next1){
        cout<<num<<' ';
    }
    cout<<endl;
    for(auto num:next2){
        cout<<num<<' ';
    }
    cout<<endl;
    return 0;
}