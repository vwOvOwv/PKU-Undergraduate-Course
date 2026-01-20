#include<iostream>
#include<stack>
#include<vector>

using namespace std;

int n;
vector<pair<string,int> >ans;
int main(){
    cin>>n;
    vector<int>v(n);
    stack<int>st;
    for(int i=0;i<n;i++){
        cin>>v[i];
        if(v[i]>n||v[i]<1){
            cout<<"NO"<<endl;
            return 0;
        }
    }

    int j=0,cur=0;
    while(j<n){
        if(cur<v[j]){
            while(cur<v[j]){
                cur++;
                st.push(cur);
                ans.push_back(make_pair("PUSH",cur));
            }
            ans.push_back(make_pair("POP",st.top()));
            st.pop();
            j++;
        }
        else if(cur==v[j]){
            ans.push_back(make_pair("POP",st.top()));
            st.pop();
            j++;
        }
        else{
            if(st.top()!=v[j]){
                cout<<"NO"<<endl;
                return 0;
            }
            else{
                ans.push_back(make_pair("POP",st.top()));
                st.pop();
                j++;
            }
        }
    }
    for(auto node:ans){
        cout<<node.first<<' '<<node.second<<endl;
    }
    return 0;
}