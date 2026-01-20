#include<iostream>
#include<vector>
using namespace std;
void get_next(vector<int>&next,string s){
    int n=s.size();
    next[0]=-1;
    int j=0,k=-1;
    while(j<n+1){
        while(k>=0&&s[j]!=s[k])
            k=next[k];
        j++;k++;
        if(j==n+1)
            break;
        next[j]=k;
    }
}
int main(){
    int caseNum=0,n;
    string s;
    while(cin>>n&&n){
        caseNum++;
        cin>>s;
        int len=s.size();
        vector<int>next(n+1,0);
        get_next(next,s);
        // for(auto num:next)
        //     cout<<num<<' ';
        // cout<<endl;
        cout<<"Test case #"<<caseNum<<endl;
        for(int i=1;i<n;i++){
            int seg=i+1-next[i+1];
            if(!((i+1)%seg)&&i+1>seg)
                cout<<i+1<<' '<<(i+1)/seg<<endl;
        }
        cout<<endl;
    }
    return 0;
}