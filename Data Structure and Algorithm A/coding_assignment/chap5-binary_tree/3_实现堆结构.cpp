#include<iostream>
#include<queue>
using namespace std;

priority_queue<int,vector<int>,greater<int> >q;

int main(){
    int t,n;
    cin>>t;
    while(t--){
        while(!q.empty())
            q.pop();
        cin>>n;
        int op,num;
        while(n--){
            cin>>op;
            if(op==1){
                cin>>num;
                q.push(num);
            }
            else{
                int ans=q.top();
                cout<<ans<<endl;
                q.pop();
            }
        }
    }
    return 0;
}