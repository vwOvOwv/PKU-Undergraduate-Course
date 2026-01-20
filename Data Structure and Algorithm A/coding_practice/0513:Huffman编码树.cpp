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
    TreeNode(int v=0,TreeNode*l=nullptr,TreeNode*r=nullptr):
        val(v),left(l),right(r){}
};

bool Compare(TreeNode*a,TreeNode*b){
    return a->val>b->val;
}

int sum=0;
void cal(TreeNode*root,int depth){
    if(!root->left&&!root->right){
        sum+=depth*root->val;
        return;
    }
    if(root->left)
        cal(root->left,depth+1);
    if(root->right)
        cal(root->right,depth+1);
}

int main(){
    int t;
    cin>>t;
    while(t--){
        int n;
        cin>>n;
        vector<TreeNode*>rec;
        int num;
        for(int i=0;i<n;i++){
            cin>>num;
            rec.push_back(new TreeNode(num));
        }
        while(rec.size()>1){
            sort(rec.begin(),rec.end(),Compare);
            TreeNode*tmp1=rec.back();
            rec.pop_back();
            TreeNode*tmp2=rec.back();
            rec.pop_back();
            TreeNode*tmp=new TreeNode(tmp1->val+tmp2->val,tmp1,tmp2);
            rec.push_back(tmp);
        }
        sum=0;
        cal(rec[0],0);
        cout<<sum<<endl;
    }
}