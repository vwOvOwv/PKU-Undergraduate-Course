#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include<stack>

using namespace std;

class TreeNode{
public:
    char val;
    TreeNode*left,*right;
    TreeNode(){}
    TreeNode(char v='\0',TreeNode*l=nullptr,TreeNode*r=nullptr):
        val(v),left(l),right(r){}
};

unordered_map<char,int>mem;
unordered_map<char,int>priority;
unordered_map<char,int>hashtable;

string trans(string midExp){
    string ans;
    int n=midExp.size();
    stack<char>st;
    for(int i=0;i<n;i++){
        if(midExp[i]=='+'||midExp[i]=='-'||midExp[i]=='*'||midExp[i]=='/'){
            while(!st.empty()&&st.top()!='('&&priority[st.top()]>priority[midExp[i]]){
                ans.push_back(st.top());
                st.pop();
            }
            st.push(midExp[i]);
        }
        else if(midExp[i]=='(')
            ans.push_back('(');
        else if(midExp[i]==')'){
            while(st.top()!='('){
                ans.push_back(st.top());
                st.pop();
            }
            st.pop();
        }
        else{
            ans.push_back(midExp[i]);
        }
    }
    while(!st.empty()){
        ans.push_back(st.top());
        st.pop();
    }
    return ans;
}

string del(string midExp){
    string ans;
    int n=midExp.size();
    for(int i=0;i<n;i++){
        if(midExp[i]=='('||midExp[i]==')')
            continue;
        ans.push_back(midExp[i]);
    }
    return ans;
}

TreeNode*buildTree(string in,string post,int inLeft,int inRight,int postLeft,int postRight){
    if(inLeft>inRight||postLeft>postRight)
        return nullptr;
    int rootPos=hashtable[post[postRight]];
    TreeNode*root=new TreeNode(in[rootPos]);
    root->left=buildTree(in,post,inLeft,rootPos-1,postLeft,postLeft+rootPos-inLeft-1);
    root->right=buildTree(in,post,rootPos+1,inRight,postLeft+rootPos-inLeft,postRight-1);
    return root;
}

// int calHeight(TreeNode*root){

// }
// int calWidth(TreeNode*root){

// }
void showTree(TreeNode*root){
    // int height=calHeight(root);
    // int width=calWidth(root);

}

int op(int num1,int num2,char oper){
    if(oper=='+')
        return num1+num2;
    if(oper=='-')
        return num1-num2;
    if(oper=='*')
        return num1*num2;
    if(oper=='/')
        return num1/num2;
}

int cal(string postExp){
    stack<int>st;
    int n=postExp.size();
    for(int i=0;i<n;i++){
        if(postExp[i]=='+'||postExp[i]=='-'||postExp[i]=='*'||postExp[i]=='/'){
            int num2=st.top();
            st.pop();
            int num1=st.top();
            st.pop();
            st.push(op(num1,num2,postExp[i]));
        }
        else{
            st.push(mem[postExp[i]]);
        }
    }
    return st.top();
}

int main(){
    string midExp;
    cin>>midExp;
    int n;
    cin>>n;
    char ch;int num;
    for(int i=0;i<n;i++){
        cin>>ch>>num;
        mem[ch]=num;
    }
    priority['+']=1;
    priority['-']=1;
    priority['*']=2;
    priority['/']=2;

    string postExp=trans(midExp);
    cout<<postExp<<endl;

    midExp=del(midExp);
    int size=midExp.size();
    for(int i=0;i<size;i++){
        hashtable[midExp[i]]=i;
    }
    TreeNode*root=buildTree(midExp,postExp,0,size-1,0,size-1);
    showTree(root);

    cout<<cal(postExp)<<endl;
}