#include<iostream>
#include<vector>
#include<unordered_map>
#include<set>
using namespace std;

//并查集，难点在于不等关系的表示
class TreeNode{
public:
    int father;
    int weight;
};

vector<TreeNode>rec;

int Find(int i){//路径压缩find
    if(rec[i].father==-1)
        return i;
    rec[i].father=Find(rec[i].father);
    return rec[i].father;
}

void Union(int i,int j){//重量权衡union
    int rt1=Find(i),rt2=Find(j);
    if(rt1!=rt2){
        if(rec[rt1].weight>=rec[rt2].weight){
            rec[rt2].father=rt1;
            rec[rt1].weight+=rec[rt2].weight;
        }
        else{
            rec[rt1].father=rt2;
            rec[rt2].weight+=rec[rt1].weight;
        }
    }
}


int main(){
    int t;
    cin>>t;
    while(t--){
        int n,m;
        cin>>n>>m;
        rec.clear();
        rec.resize(2*n+1);//多开一倍长度，多出来的部分与原有部分形成等价类，只是这种情况下的“跨越等价关系”意味着不在一个团伙

        for(int i=0;i<2*n+1;i++){//初始化父指针表
            rec[i].father=-1;
            rec[i].weight=1;
        }
        for(int i=0;i<m;i++){
            char ch;int crime1,crime2;
            cin>>ch>>crime1>>crime2;
            if(ch=='A'){
                if(Find(crime1)==Find(crime2))
                    cout<<"In the same gang."<<endl;
                else if(Find(crime1)==Find(crime2+n))//Find(crime1+n)==Find(crime2)也可
                    cout<<"In different gangs."<<endl;
                else
                    cout<<"Not sure yet."<<endl;
            }
            else{
                //相反的关系存在相反的集合里
                Union(crime1,crime2+n);
                Union(crime1+n,crime2);
            }
        }
    }
    return 0;
}