#include<iostream>
#include<vector>
#include<unordered_map>
using namespace std;

vector<int>inorder,postorder,order,preorder;
unordered_map<int,int>hash_table;
class TreeNode{
public:
    int val;
    TreeNode*left,*right;
    TreeNode(int v=0,TreeNode*l=nullptr,TreeNode*r=nullptr):
        val(v),left(l),right(r){}
};

TreeNode*buildTree(vector<int>inorder,vector<int>postorder,int postleft,int postright,int inleft,int inright){
    if(postleft>postright)
        return nullptr;
    int root_in_pos=hash_table[postorder[postright]];
    TreeNode*root=new TreeNode(postorder[postright]);
    root->left=buildTree(inorder,postorder,postleft,postleft+root_in_pos-1-inleft,inleft,root_in_pos-1);
    root->right=buildTree(inorder,postorder,postleft+root_in_pos-inleft,postright-1,root_in_pos+1,inright);//注意计算
    return root;
}

void preOrder(TreeNode*root){
    if(root)
        preorder.push_back(root->val);
    if(root)
        preOrder(root->left);
    if(root)
        preOrder(root->right);
}

int main(){
    int num;
    while(cin>>num){
        order.push_back(num);
    }
    int n=order.size();
    for(int i=0;i<n;i++){
        if(i<n/2)
            inorder.push_back(order[i]);
        else
            postorder.push_back(order[i]);
    }
    n/=2;
    for(int i=0;i<n;i++)
        hash_table[inorder[i]]=i;//重要的简化
    TreeNode*root=buildTree(inorder,postorder,0,n-1,0,n-1);
    preOrder(root);
    for(int i=0;i<n;i++){
        cout<<preorder[i];
        if(i<n-1)
            cout<<' ';
    }
    return 0;
}