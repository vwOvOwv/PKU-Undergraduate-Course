#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>

using namespace std;

unordered_map<int,int>hashtable;

class TreeNode{
public:
    int val;
    TreeNode*left,*right;
    TreeNode(){}
    TreeNode(int v=0,TreeNode*l=nullptr,TreeNode*r=nullptr):val(v),left(l),right(r){}
};

TreeNode*buildTree(vector<int>in,vector<int>post,int inLeft,int inRight,int postLeft,int postRight){
    if(inLeft>inRight||postLeft>postRight)
        return nullptr;
    int rootPos=hashtable[post[postRight]];
    TreeNode*root=new TreeNode(post[postRight]);
    root->left=buildTree(in,post,inLeft,rootPos-1,postLeft,postLeft+rootPos-inLeft-1);
    root->right=buildTree(in,post,rootPos+1,inRight,postLeft+rootPos-inLeft,postRight-1);
    return root;
}

vector<int>ans;
void preOrder(TreeNode*root){
    if(root){
        ans.push_back(root->val);
        preOrder(root->left);
        preOrder(root->right);
    }
}

int main(){
    vector<int>nums;
    vector<int>mid,post;
    int num;
    while(cin>>num)
        nums.push_back(num);
    int n=nums.size();
    for(int i=0;i<n/2;i++){
        mid.push_back(nums[i]);
        hashtable[nums[i]]=i;
    }
    for(int i=n/2;i<n;i++){
        post.push_back(nums[i]);
    }
    n/=2;
    TreeNode*root=buildTree(mid,post,0,n-1,0,n-1);
    preOrder(root);
    int size=ans.size();
    for(int i=0;i<size;i++){
        cout<<ans[i];
        if(i<size-1)
            cout<<' ';
    }
    cout<<endl;
    return 0;
}