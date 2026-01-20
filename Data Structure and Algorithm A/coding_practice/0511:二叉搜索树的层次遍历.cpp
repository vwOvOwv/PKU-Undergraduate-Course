#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<queue>

using namespace std;

class TreeNode{
public:
    int val;
    TreeNode*left,*right;
    TreeNode(){}
    TreeNode(int v=0,TreeNode*l=nullptr,TreeNode*r=nullptr):val(v),left(l),right(r){}
};

vector<int>ans;

void bfs(TreeNode*root){
    queue<TreeNode*>q;
    q.push(root);
    while(!q.empty()){
        TreeNode*p=q.front();
        ans.push_back(p->val);
        if(p->left)
            q.push(p->left);
        if(p->right)
            q.push(p->right);
        q.pop();
    }
}

int main(){
    vector<int>rec;
    int num;
    while(cin>>num)
        rec.push_back(num);
    TreeNode*root=new TreeNode(rec[0]);
    int n=rec.size();
    for(int i=1;i<n;i++){
        TreeNode*tmp=new TreeNode(rec[i]);
        TreeNode*pointer=root;
        // cout<<"called "<<i<<endl;//
        while(1){
            if(tmp->val<pointer->val){
                if(pointer->left)
                    pointer=pointer->left;
                else{
                    pointer->left=tmp;
                    break;
                }
            }
            else if(tmp->val>pointer->val){
                if(pointer->right)
                    pointer=pointer->right;
                else{
                    pointer->right=tmp;
                    break;
                }
            }
            else
                break;
        }
    }
    bfs(root);
    int size=ans.size();
    for(int i=0;i<size;i++){
        cout<<ans[i];
        if(i<size-1)
            cout<<' ';
    }
}