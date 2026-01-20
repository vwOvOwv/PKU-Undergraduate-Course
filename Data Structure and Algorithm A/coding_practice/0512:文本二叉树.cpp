#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

class TreeNode{
public:
    char val;
    TreeNode*left,*right;
    int depth;
    TreeNode(){}
    TreeNode(char v,TreeNode*l,TreeNode*r,int d):val(v),left(l),right(r),depth(d){}
};

string pre,in,post;

void preOrder(TreeNode*root){
    if(root){
        if(root->val!='*')
            pre.push_back(root->val);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(TreeNode*root){
    if(root){
        inOrder(root->left);
        if(root->val!='*')
            in.push_back(root->val);
        inOrder(root->right);
    }
}

void postOrder(TreeNode*root){
    if(root){
        postOrder(root->left);
        postOrder(root->right);
        if(root->val!='*')
            post.push_back(root->val);
    }
}

int main(){
    int n;
    cin>>n;
    while(n--){
        string s;
        vector<TreeNode*>rec;
        pre.clear();
        in.clear();
        post.clear();
        while(cin>>s){
            if(s=="0")
                break;
            rec.push_back(new TreeNode(s.back(),nullptr,nullptr,s.size()-1));
        }
        int size=rec.size();
        TreeNode*root=rec[0];
        for(int i=0;i<size-1;i++){
            if(rec[i]->val=='*')
                continue;
            int countChild=0;
            for(int j=i+1;j<size;j++){
                if(rec[j]->depth==rec[i]->depth+1){
                    countChild++;
                    if(countChild==1){
                        rec[i]->left=rec[j];
                    }
                    else if(countChild==2){
                        rec[i]->right=rec[j];
                        break;
                    }
                }
                else if(rec[j]->depth==rec[i]->depth)
                    break;
            }
        }
        preOrder(root);
        postOrder(root);
        inOrder(root);
        cout<<pre<<endl<<post<<endl<<in<<endl;
        cout<<endl;
    }
}