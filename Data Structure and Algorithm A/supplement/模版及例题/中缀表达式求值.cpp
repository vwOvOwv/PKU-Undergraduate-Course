#include<iostream>
#include<stack>
#include<unordered_map>
#include<math.h>
using namespace std;

stack<int>num;
stack<char>op;
unordered_map<char,int>priority;
void calculate(int num1,int num2,char oper){
    switch (oper){
        case '+':num.push(num1+num2);break;
        case '-':num.push(num1-num2);break;
        case '*':num.push(num1*num2);break;
        case '/':num.push(num1/num2);break;
        case '^':num.push(pow(num1,num2));break;
    }
}

int n;
int main(){
    priority['+']=priority['-']=1;
    priority['*']=priority['/']=2;
    priority['^']=3;
    cin>>n;
    cin.get();
    while(n--){
        while(!num.empty())
            num.pop();
        while(!op.empty())
            op.pop();
        while(1){
            char c=cin.peek();
            if(c=='\n'||c==EOF||c=='\r'){
                cin.get();
                if(c=='\r')
                    cin.get();
                break;
            }
            else if(c==' '){
                cin.get();
            }
            else if(c=='('){
                cin.get();
                op.push('(');
            }
            else if(c==')'){
                cin.get();
                while(op.top()!='('){
                    char oper=op.top();
                    op.pop();
                    int num2=num.top();num.pop();
                    int num1=num.top();num.pop();
                    calculate(num1,num2,oper);
                }
                op.pop();
            }
            else if(c=='+'||c=='-'||c=='*'||c=='/'||c=='^'){
                cin.get();
                if(c=='-'&&cin.peek()!=' '){
                    int number;
                    cin>>number;
                    num.push(-number);
                    continue;
                }
                while(!op.empty()&&op.top()!='('&&priority[op.top()]>=priority[c]){
                    char oper=op.top();
                    op.pop();
                    int num2=num.top();num.pop();
                    int num1=num.top();num.pop();
                    calculate(num1,num2,oper);
                }
                op.push(c);
            }
            else{
                int number;
                cin>>number;
                num.push(number);
            }
        }
        while(!op.empty()){
            char oper=op.top();
            op.pop();
            int num2=num.top();num.pop();
            int num1=num.top();num.pop();
            calculate(num1,num2,oper);
        }
        cout<<num.top()<<endl;
    }
    return 0;
}