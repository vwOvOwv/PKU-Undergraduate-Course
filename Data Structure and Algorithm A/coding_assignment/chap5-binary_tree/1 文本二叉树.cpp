#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
using namespace std;

class TreeNode{
public:
    char val;
    TreeNode*left,*right;
    TreeNode(char v='\0',TreeNode*l=nullptr,TreeNode*r=nullptr):
        val(v),left(l),right(r){}
};


int cnt(string s){
    int n=s.size();
    int i=0;
    for(;s[i]=='-';i++);
    return i;
}

string preOrder,inOrder,postOrder;

void preRoot(TreeNode*root){
    if(root){
        if(root->val!='*')
            preOrder.push_back(root->val);
        preRoot(root->left);
        preRoot(root->right);
    }
}

void inRoot(TreeNode*root){
    if(root){
        inRoot(root->left);
        if(root->val!='*')
            inOrder.push_back(root->val);
        inRoot(root->right);
    }
}

void postRoot(TreeNode*root){
    if(root){
        postRoot(root->left);
        postRoot(root->right);
        if(root->val!='*')
            postOrder.push_back(root->val);
    }
}
int main(){
   int n;
   cin>>n;
   while(n--){
        string s;
        preOrder.clear();inOrder.clear();postOrder.clear();
        vector<pair<TreeNode*,int> >rec;
        while(cin>>s){
            if(s=="0")
                break;
            TreeNode*p=new TreeNode(s.back());
            int d=cnt(s);
            rec.push_back(make_pair(p,d));
        }
        int size=rec.size();
        for(int i=0;i<size-1;i++){
            if(rec[i].first->val=='*')   
                continue;
            int countChild=0;
            for(int j=i+1;j<size;j++){
                if(rec[j].second==rec[i].second+1){
                    countChild++;
                    if(countChild==1){
                        rec[i].first->left=rec[j].first;
                    }
                    else if(countChild==2){
                        rec[i].first->right=rec[j].first;
                    }
                    else
                        break;
                }
                else if(rec[j].second==rec[i].second)//要找最近的子结点！！！不能抢了别人的子结点
                    break;
            }
        }
        preRoot(rec[0].first);
        inRoot(rec[0].first);
        postRoot(rec[0].first);
        cout<<preOrder<<endl<<postOrder<<endl<<inOrder<<endl;
        if(n>=1)
            cout<<endl;
   }
   return 0;
}
