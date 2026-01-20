#include<iostream>
#include<stack>

using namespace std;

long long int factor(int n){//label0 非递归入口
    if(n==0)
        return 1;
    else//rd1
        return n*factor(n-1);//label1
}//label2

class stackNode{
public:
    long long int res;//mark
    int n,rd;
};
long long  nonRecFactor(int n){
    stack<stackNode>st;
    stackNode tmp,x;
    tmp.n=n,tmp.rd=0;
    st.push(tmp);//栈底监视哨
label0:
    tmp=st.top();
    st.pop();
    if(tmp.n==0){
        tmp.res=1;
        st.push(tmp);
        goto label2;
    }
    st.push(tmp);
    x.n=tmp.n-1;
    x.rd=1;
    st.push(x);
    goto label0;
label1:
    x=st.top();
    st.pop();
    x.res=tmp.res*x.n;
    st.push(x);
label2:
    tmp=st.top();
    st.pop();
    if(tmp.rd)
        goto label1;
    else
        return tmp.res;
}
int main(){
    long long int k;
    while(cin>>k){
        cout<<factor(k)<<endl;
        cout<<nonRecFactor(k)<<endl;
    }
    return 0;
}