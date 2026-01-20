#include<iostream>
#include<stack>
using namespace std;

int fib(int n){
    if(n==1||n==2)
        return 1;
    int res1=fib(n-1);
    int res2=fib(n-2);
    return res1+res2;
}

class stackNode{
public:
    int res,rd,n;
};
int nonRecFib(int n){
    stack<stackNode>st;
    stackNode x,tmp;
    tmp.rd=0,tmp.n=n;
    st.push(tmp);
label0:
    tmp=st.top();
    st.pop();
    if(tmp.n==1||tmp.n==2){
        tmp.res=1;
        st.push(tmp);//！！！
        goto label3;
    }
    st.push(tmp);
    x.n=tmp.n-1;
    x.rd=1;
    st.push(x);
    goto label0;
label1:
    x=st.top();
    st.pop();
    x.res=tmp.res;
    st.push(x);
    tmp.n=x.n-2;
    tmp.rd=2;
    st.push(tmp);
    goto label0;
label2:
    x=st.top();
    st.pop();
    x.res+=tmp.res;
    st.push(x);
label3:
    tmp=st.top();
    st.pop();
    switch (tmp.rd){
        case 0:return tmp.res;
        case 1:goto label1;
        case 2:goto label2;
    }
}

int main(){
    int n;
    while(cin>>n){
        cout<<fib(n)<<endl;
        cout<<nonRecFib(n)<<endl;
    }
    return 0;
}