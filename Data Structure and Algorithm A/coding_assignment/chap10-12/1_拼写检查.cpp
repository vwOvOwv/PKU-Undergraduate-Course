#include<iostream>
#include<string>
#include<vector>

using namespace std;

vector<string>dic;

int main(){
    string s;
    while(cin>>s){
        if(s=="#")
            break;
        dic.push_back(s);
    }
    int n=dic.size();
    while(cin>>s){
        if(s=="#")
            break;
        vector<string>ans;
        bool found=false;
        for(int i=0;i<n;i++){
            if(s==dic[i]){
                found=true;
                break;
            }
            int l1=s.size(),l2=dic[i].size();
            int delta=abs(l1-l2);
            if(delta>1)
                continue;
            if(delta==1){
                bool changed=false;
                int j=0,k=0;
                while(j<l1&&k<l2){
                    if(s[j]==dic[i][k]){
                        j++,k++;
                    }
                    else{
                        if(!changed){
                            l1<l2?k++:j++;
                            changed=true;
                        }
                        else
                            break;
                    }
                }
                if(j==l1||k==l2)
                    ans.push_back(dic[i]);
            }
            else{
                int cnt=0,j;
                for(j=0;j<l1;j++){
                    if(s[j]!=dic[i][j]){
                        cnt++;
                        if(cnt>=2)
                            break;
                    }
                }
                if(j==l1)
                    ans.push_back(dic[i]);
            }
        }
        if(found)
            cout<<s<<" is correct"<<endl;
        else{
            cout<<s<<": ";
            int m=ans.size();
            for(int i=0;i<m;i++){
                cout<<ans[i];
                if(i<m-1)
                    cout<<' ';
            }
            cout<<endl;
        }
    }
    return 0;
}