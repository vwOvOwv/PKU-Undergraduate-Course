#include <cstddef>
#include <iostream>

using namespace std;

int n, p, m;

class listNode{
public:
    int val;
    listNode *next;
    listNode(){}
    listNode(int val=0, listNode *next=nullptr)
};

void josephus(int n, int p, int m){
    listNode *cur = nullptr;
    for (int i = 1; i<= n; i++){
        listNode node;
    }
}

int main(){
    while(cin >> n >> p >> m){
        if(n == 0 && p == 0 && m == 0){
            return 0;
        }
        josephus(n, p, m);
    }
}