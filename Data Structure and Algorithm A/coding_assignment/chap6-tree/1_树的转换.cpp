#include<iostream>
#include<string>
#include<stack>
#include<algorithm>
#include<vector>
#include<queue>
using namespace std;

class TreeNode{
public:
    vector<TreeNode*>children;
};

int BinaryTreeHeight(TreeNode*root){
    if(!root)
        return 0;
    int ans=0;
    int n=root->children.size();
    for(int i=0;i<n;i++){
        ans=max(ans,BinaryTreeHeight(root->children[i])+i+1);//重要！
    }
    return ans;
}

int TreeHeight(TreeNode*root){
    int ans=0;
    if(!root)
        return ans;
    int n=root->children.size();
    for(int i=0;i<n;i++){
        ans=max(ans,TreeHeight(root->children[i])+1);
    }
    return ans;
}
int main(){
    string tree;
    int t=0;
    while(cin>>tree){
        if(tree=="#")
            return 0;
        t++;
        int n=tree.length();
        //利用栈序列转换成树
        stack<TreeNode*>rec;
        TreeNode*root=new TreeNode;
        rec.push(root);
        for(int i=0;i<n;i++){
            if(tree[i]=='d'){
                rec.top()->children.push_back(new TreeNode);
                rec.push(rec.top()->children.back());
            }
            else
                rec.pop();
        }
        //根据子结点表表示的树求二叉树高
        int binaryTreeHeight=BinaryTreeHeight(root);
        //求树高
        int treeHeight=TreeHeight(root);
        //输出答案
        cout<<"Tree "<<t<<": "<<treeHeight<<" => "<<binaryTreeHeight<<endl;
    }
    return 0;
}