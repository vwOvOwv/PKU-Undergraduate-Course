#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

class TreeNode{
public:
    int val;
    TreeNode*left,*right;
    TreeNode(){}
    TreeNode(int v=0,TreeNode*l=nullptr,TreeNode*r=nullptr):val(v),left(l),right(r){}
};

vector<int>ans;

void preOrder(TreeNode*root){
    if(root){
        ans.push_back(root->val);
        preOrder(root->left);
        preOrder(root->right);
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
    preOrder(root);
    int size=ans.size();
    for(int i=0;i<size;i++){
        cout<<ans[i];
        if(i<size-1)
            cout<<' ';
    }
    cout<<endl;
}