#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

class TreeNode{
public:
    int father,weight;
    TreeNode(){}
    TreeNode(uint f,int w):father(f),weight(w){}
};

vector<TreeNode>rec;

int Find(int i){
    if(rec[i].father==i)
        return i;
    rec[i].father=Find(rec[i].father);
    return rec[i].father;
}

void Union(int i,int j){
    int rt1=Find(i),rt2=Find(j);
    if(rt1!=rt2){
        if(rec[rt1].weight<rec[rt2].weight){
            rec[rt1].father=rt2;
            rec[rt2].weight+=rec[rt1].weight;
        }
        else{
            rec[rt2].father=rt1;
            rec[rt1].weight+=rec[rt2].weight;
        }
    }
}

int main(){
    int n,m;
    int t=0;
    while(cin>>n>>m){
        if(n==0&&m==0)
            break;
        // cout<<n<<' '<<m<<endl;
        t++;
        rec.resize(n);
        for(int i=0;i<n;i++){
            rec[i].father=i;
            rec[i].weight=1;
        }
        int i,j;
        int equNum=n;
        for(int k=0;k<m;k++){
            cin>>i>>j;
            if(Find(i)!=Find(j)){
                Union(i,j);
                equNum--;
            }
        }
        cout<<"Case "<<t<<": "<<equNum<<endl;
    }
}