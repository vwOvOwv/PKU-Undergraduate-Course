#include<iostream>
#include<vector>
#include<string>
#include<set>
#include<unordered_map>

using namespace std;

bool matched(string a,string b){
    int l1=a.size(),l2=b.size();
    if(l1!=l2)
        return false;
    int index1=0,index2=0;
    while(index1<l1&&index2<l2){
        if(a[index1]>='A'&&a[index1]<='Z')
            a[index1]=a[index1]-'A'+'a';
        if(b[index2]>='A'&&b[index2]<='Z')
            b[index2]=b[index2]-'A'+'a';
        if(a[index1]==b[index2]){
            index1++,index2++;
        }
        else
            return false;
    }
    return true;
}

int main(){
    int n;
    cin>>n;
    vector<string>rec(n+1);
    for(int i=1;i<=n;i++){
        cin>>rec[i];
    }
    string models;
    cin>>models;
    string model;
    int size=models.size();
    int left=models.find('['),right=models.find(']');
    model=models.substr(0,left);
    for(int i=left+1;i<right;i++){
        string tmp=model+models[i]+models.substr(right+1);
        // cout<<tmp<<endl;
        for(int j=1;j<=n;j++){
            if(matched(tmp,rec[j]))
                cout<<j<<' '<<rec[j]<<endl;
        }
    }
}