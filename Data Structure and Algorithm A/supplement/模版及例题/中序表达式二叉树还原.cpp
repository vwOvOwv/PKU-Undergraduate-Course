#include<iostream>
using namespace std;
class TreeNode{
public:
    char val;
    TreeNode*left,*right;
    TreeNode(char v,TreeNode*l=nullptr,TreeNode*r=nullptr){
        val=v;
        left=l;
        right=r;
    }
};
class expression{
public:
    TreeNode*root;
    expression(){
        root=new TreeNode('-');
        root->left=new TreeNode('a');
        root->right=new TreeNode('*');
        TreeNode*tmp=root->right;
        tmp->left=new TreeNode('/');
        tmp->right=new TreeNode('d');
        tmp=tmp->left;
        tmp->left=new TreeNode('b');
        tmp->right=new TreeNode('c');
    }
};


string ans;
void dfs(TreeNode*root,int flag){
    if(root){
        char c=root->val;
        if(c=='+'||c=='-'||c=='*'||c=='/'){
            switch (c)
            {
            case '+':
                if(ans.back()=='*'||ans.back()=='/'||ans.back()=='-'){//检查上一个运算符
                    flag=1;
                    ans.push_back('(');
                }
                break;
            case '-':
                if(ans.back()=='*'||ans.back()=='/'||ans.back()=='-'){
                    flag=1;
                    ans.push_back('(');
                }
                break;
            case '*':
            }
        }
        dfs(root->left,0);
        ans.push_back(root->val);
        dfs(root->right,0);
        if(flag){
            ans.push_back(')');
        }
    }
}
int main(){
    expression ex;
    TreeNode*root=ex.root;
    dfs(root,0);
    cout<<ans;
}