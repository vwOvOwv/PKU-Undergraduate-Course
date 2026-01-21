#include<iostream>
#include<vector>
#include<algorithm>
#include<set>

using namespace std;

int main(){
    int t;
    int n;
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        int num;
        vector<int>ans;
        set<int>rec;
        for(int i=0;i<n;i++){
            scanf("%d",&num);
            if(!rec.count(num)){
                ans.push_back(num);
                rec.insert(num);
            }
        }
        int size=ans.size();
        for(int i=0;i<size;i++){
            cout<<ans[i];
            if(i<size-1)
                cout<<' ';
        }
        cout<<endl;
    }
    return 0;
}