#include<iostream>
#include<vector>
#include<queue>
#include<cmath>
#include<iomanip>
#include<algorithm>
#include<stack>

using namespace std;

bool found;
int target;

char getValidChar(){
    char c=getchar();
    while(c==' '||c=='\n')
        c=getchar();
    return c;
}

bool dfs(int sum){
    char c;
    c=getValidChar();
    c=getValidChar();
    if(c==')')
        return false;
    int minus=1;
    if(c=='-'){
        minus=-1;
        c=getValidChar();
    }
    int val=0;
    while(c>='0'&&c<='9'){
        val=val*10+c-'0';
        c=getValidChar();
    }
    ungetc(c,stdin);
    val=minus*val;
    sum+=val;
    bool left=dfs(sum),right=dfs(sum);
    if(!left&&!right){
        if(sum==target)
            found=true;
    }
    c=getValidChar();
    return true;
}

int main(){
    char c;
    while(cin.peek()!=EOF){
        found=false;
        cin>>target;
        dfs(0);
        if(found)
            cout<<"yes"<<endl;
        else
            cout<<"no"<<endl;
        getchar();
    }
    return 0;
}