#include<iostream>
#include<stack>

using namespace std;

int w[5]={1,2,3,4,5};
bool knap(int s,int n){//递归调用入口 label0
    if(s==0)
        return true;//递归终点出口1 rd=0
    if((s<0)||(s>0&&n<1))
        return false;//递归终点出口2 rd=0
    if(knap(s-w[n-1],n-1)){//规则1 rd=1
        cout<<w[n-1]<<' ';//规则1对应的返回处理 label1
        return true;
    }
    else return knap(s,n-1);//规则2 rd=2 //规则2对应的返回处理 label2
}//递归出口3 label3

class stackNode{
public:
    int s,n;
    int rd;
    bool res;
};
stack<stackNode>st;//创立工作栈
bool nonRecKnap(int s,int n){
    stackNode tmp;
    stackNode x;
    tmp.s=s,tmp.n=n,tmp.rd=0;
    st.push(tmp);//第一次调用递归函数/非递归调用入口
label0://递归调用入口
    tmp=st.top();
    st.pop();
    if(tmp.s==0){//满足出口1
        tmp.res=true;//把结果存入栈顶元素
        st.push(tmp);
        goto label3;//转向递归出口处理
    }
    if(tmp.s<0||(tmp.s>0&&tmp.n<1)){//满足出口2
        tmp.res=false;//把结果存入栈顶元素
        st.push(tmp);
        goto label3;//转向递归出口处理
    }
    st.push(tmp);//尚未找到递归出口,原样放回去继续执行
    x.s=tmp.s-w[tmp.n-1];//按照规则1压栈
    x.n=tmp.n-1;
    x.rd=1;
    st.push(x);
    goto label0;
label1://规则1对应的返回处理
    x=st.top();
    st.pop();
    if(tmp.res==true){//下一层为true
        x.res=true;//把结果上传给调用方
        st.push(x);
        cout<<w[x.n-1]<<' ';
        goto label3;
    }
    st.push(x);//下一层结果为false
    tmp.s=x.s;//选择当前物品不合适，回溯，调用规则2
    tmp.n=x.n-1;
    tmp.rd=2;
    st.push(tmp);
    goto label0;
label2://规则2对应的返回处理
    x=st.top();
    st.pop();
    x.res=tmp.res;
    st.push(x);
label3:
    tmp=st.top();
    st.pop();
    switch (tmp.rd)
    {
    case 0: return tmp.res;//算法结果并返回结果
    case 1: goto label1;//转向规则1的返回处理处
    case 2: goto label2;//转向规则2的返回处理处
    }
}
int main(){
    cout<<knap(10,5)<<endl;
    cout<<nonRecKnap(17,5)<<endl;
    return 0;
}