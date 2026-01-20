#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

class TreeNode{
public:
    int father,weight;
    TreeNode(){}
    TreeNode(int f,int w):father(f),weight(w){}
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
            rec[rt2].weight+=rec[rt1].weight;
            rec[rt1].father=rt2;
        }
        else{
            rec[rt1].weight+=rec[rt2].weight;
            rec[rt2].father=rt1;
        }
    }
}

int main(){
    int n,k;
    cin>>n>>k;
    rec.resize(3*n+1);
    for(int i=1;i<=3*n;i++){
        rec[i].father=i;
        rec[i].weight=1;
    }

    int fake=0;
    for(int i=0;i<k;i++){
        int num,x,y;
        cin>>num>>x>>y;
        if(x<1||x>n||y<1||y>n){
            fake++;
            continue;
        }
        if(num==1){
            if(Find(x)==Find(y+n)||Find(x+n)==Find(y))
                fake++;
            else{
                Union(x,y);
                Union(x+n,y+n);
                Union(x+2*n,y+2*n);
            }
        }
        else{
            if(Find(x)==Find(y)||Find(y)==Find(x+n))
                fake++;
            else{
                Union(x,y+n);
                Union(x+n,y+2*n);
                Union(x+2*n,y);
            }
        }
    }
    cout<<fake<<endl;
}