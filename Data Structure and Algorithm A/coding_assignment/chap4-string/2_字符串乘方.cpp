#include<iostream>
#include<string>
#include<vector>
#include<cstring>
using namespace std;
char s[1000002];
vector<int>Next;
int main(){
    while(scanf("%s",&s)){
        if(!strcmp(s,"."))
            return 0;
        int len=strlen(s);
        Next.clear();
        Next.resize(len*sizeof(int));
        Next[0]=-1;
        int j=0,k=-1;
        while(j<len){
            while(k>=0&&s[j]!=s[k])
                k=Next[k];
            k++,j++;
            Next[j]=k;
        }
        int tag=len-Next[len];
        if(len%tag==0)
            printf("%d\n",len/tag);
        else//形如ababa的，首尾配串有重叠
            printf("%d\n",1);
    }
}