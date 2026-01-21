#include<iostream>
#include<stack>
#include<vector>

using namespace std;

stack<int>st;
vector<int>v;
vector<pair<string,int> >ans;
int n;
int main(){
    cin>>n;
    int num;
    for(int i=0;i<n;i++){
        cin>>num;
        if(num>n||num<1){//判断错误输入
            cout<<"NO"<<endl;
            return 0;
        }
        v.push_back(num);
    }
    int now=0;
    for(auto num:v){
        if(num>now){
            while(num>now){
                now++;
                st.push(now);
                ans.push_back(make_pair("PUSH",now));
            }
            st.pop();
            ans.push_back(make_pair("POP",num));
        }
        else if(num<=now){
            if(st.top()!=num){
                cout<<"NO"<<endl;
                return 0;
            }
            else{
                st.pop();
                ans.push_back(make_pair("POP",num));
            }
        }
    }
    for(auto node:ans){
        cout<<node.first<<' '<<node.second<<endl;
    }
    return 0;
}