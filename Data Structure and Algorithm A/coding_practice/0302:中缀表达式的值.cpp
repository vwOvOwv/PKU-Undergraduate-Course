#include<iostream>
#include<stack>
#include<string>
#include<vector>
#include<unordered_map>

using namespace std;

int n;
stack<char>st;
vector<int>postExpression;
unordered_map<char,int>hashtable,priority;

int calculate(vector<int>post){
    stack<int>st;
    int n=post.size();
    for(int i=0;i<n;i++){
        if(post[i]>0){
            st.push(post[i]);
        }
        else{
            int num1=st.top();
            st.pop();
            int num2=st.top();
            st.pop();
            switch (post[i])
            {
                case -1:st.push(num1+num2);break;
                case -2:st.push(num2-num1);break;
                case -3:st.push(num2*num1);break;
                case -4:st.push(num2/num1);break;
            }
        }
    }
    return st.top();
}

int main(){
    cin>>n;
    
    hashtable['+']=-1;
    hashtable['-']=-2;
    hashtable['*']=-3;
    hashtable['/']=-4;
    priority['+']=1;
    priority['-']=1;
    priority['*']=2;
    priority['/']=2;
    while(n--){
        char c;
        postExpression.clear();
        string s;
        cin>>s;
        for(int i=0;i<s.size();i++){
            c=s[i];
            if(c=='+'||c=='-'||c=='*'||c=='/'){
                while(!st.empty()&&st.top()!='('&&priority[st.top()]>=priority[c]){//栈非空 栈顶不是左括号 栈顶符号优先级大于等于当前符号优先级
                    char tmp=st.top();
                    st.pop();
                    postExpression.push_back(hashtable[tmp]);
                }
                st.push(c);
            }
            else if(c=='('){
                st.push(c);
            }
            else if(c==')'){
                while(st.top()!='('){
                    char tmp=st.top();
                    st.pop();
                    postExpression.push_back(hashtable[tmp]);
                }
                st.pop();
            }
            else{
                int num=0;     
                for(;s[i]>='0'&&s[i]<='9';i++){
                    num=num*10+(s[i]-'0');
                }
                i--;//回退一位
                postExpression.push_back(num);          
            }
        }
        while(!st.empty()){//栈中剩下的符号全弹出
            postExpression.push_back(hashtable[st.top()]);
            st.pop();
        }
        cout<<calculate(postExpression)<<endl;
    }
    return 0;
}