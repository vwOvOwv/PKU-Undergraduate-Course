#include<iostream>
#include<stack>
using namespace std;

bool found;
int target;

char getValidChar(){//对于简化思路是重要的！
    char c=getchar();
    while(c==' '||c=='\n')
        c=getchar();
    return c;
}
bool dfs(int sum){//用返回值传递结点是否为空！true为空，false为非空
    // if(found)
    //     return false;
    //不能在这里剪枝，否则读不完！！！
    char c;
    c=getValidChar();//左括号
    c=getValidChar();//右括号/负号/数字

    if(c==')')//空树
        return true;
    //求数字的值
    int minus=1,val=0;
    if(c=='-'){
        minus=-1;//trick
        c=getValidChar();
    }
    while(c>='0'&&c<='9'){
        val=val*10+(c-'0');
        c=getValidChar();
    }
    //多读了一位，放回去！
    ungetc(c,stdin);
    //更新sum
    sum+=minus*val;
    //前序遍历框架
    bool left=dfs(sum),right=dfs(sum);
    if(left&&right){//是叶结点
        if(sum==target)
            found=true;
    }
    c=getValidChar();//读掉右括号
    return false;//非空
}

int main(){
    while(cin.peek()!=EOF){
        found=false;
        cin>>target;
        dfs(0);
        if(found)
            cout<<"yes\n";
        else
            cout<<"no\n";
        getchar();//读掉末尾回车！！！
    }
    return 0;
}