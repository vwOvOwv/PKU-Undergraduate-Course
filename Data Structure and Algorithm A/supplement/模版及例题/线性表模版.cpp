#include<iostream>

using namespace std;

template<class T>
class List{//线性表类定义
public:
    void clear();
    bool isEmpty();
    bool append(const T value);
    bool insert(const int p,const T value);
    bool myDelete(const int p);
    bool getPos(int&p,const T value);
    bool getValue(const int p,T&value);
    bool setValue(const int p,const T value);
};

template<class T>
class arrList:public List<T>{//顺序表（向量）类定义
private:
    T*aList;//存储顺序表的实例
    int maxLen;//顺序表实例的最大长度
    int curLen;//顺序表实例的当前长度
    int curPos;//当前处理位置
public:
    arrList(const int length){
        maxLen=length;
        aList=new T[maxLen];
        curLen=0;
        curPos=0;
    }
    ~arrList(){
        delete []aList;
    }
    void clear(){
        delete []aList;//清空顺序表
        curLen=0;
        curPos=0;
        aList=new T[maxLen];//重建一个新的
    }
    int length(){
        return curLen;
    }
    bool append(const T value){
        if(curLen>=maxLen){
            cout<<"List Overflow!"<<endl;
            return false;
        }
        else{
            aList[curLen]=value;
            curLen++;
            return true;
        }
    }
    bool insert(const int p,const T value){
        if(curLen>=maxLen){
            cout<<"List Overflow!"<<endl;
            return false;
        }
        else if(p<0||p>=curLen){
            cout<<"Wrong Input!"<<endl;
            return false;
        }
        else{
            for(int i=curLen-1;i>=p;i--){//移动curLen-p次
                aList[i+1]=aList[i];
            }
            curLen++;
            aList[p]=value;
            return true;
        }
    }
    bool myDelete(const int p){
        if(p<0||p>=curLen){
            cout<<"Wrong Input!"<<endl;
            return false;
        }
        else{
            for(int i=p;i<curLen-1;i++){//移动curlen-p-1次
                aList[i]=aList[i+1];
            }
            curLen--;
            return true;
        }
    }
    bool setValue(const int p,const T value){
        if(p<0||p>=curLen){
            cout<<"Wrong Input!"<<endl;
            return false;
        }
        else{
            aList[p]=value;
            return true;
        }
    }
    bool getValue(const int p,T&value){
        if(p<0||p>=curLen){
            cout<<"Wrong Input!"<<endl;
            value=-1;
            return false;
        }
        else{
            value=aList[p];
            return true;
        }
    }
    bool getPos(int&p,const T value){
        for(int i=0;i<curLen;i++){
            if(aList[i]==value){
                p=i;
                return true;
            }
        }
        p=-1;
        return false;
    }
    void print(){
        for(int i=0;i<curLen;i++){
            cout<<aList[i]<<' ';
        }
        cout<<endl;
        cout<<"curLen="<<curLen<<endl;
    }
};

template<class T>
class Link{//单向链表的结点类型
public:
    T data;
    Link<T>* next;
    Link(const T info,Link<T>* nextPtr=NULL){
        data=info;
        next=nextPtr;
    }
    Link(Link<T>* nextPtr=NULL){
        next=nextPtr;
    }
};
template<class T>
class lnkList:public List<T>{//带头结点的单向链表类定义
private:
    Link<T>*head,*tail;
    Link<T>*setPos(const int i){//返回线性表第i-1个元素的next指针值
        int cnt=0;
        if(i==-1)//i为-1则定位到头结点
            return head;
        Link<T>*p=head->next;
        while(p!=NULL&&cnt<i){//i为0时定位到第一个结点
            p=p->next;
            cnt++;
        }
        return p;//p指向第i个结点，当链表结点数小于i时返回NULL
    }
public:
    lnkList(){//带有头结点的单链表
        head=tail=new Link<T>;
    }
    ~lnkList(){//清除链表
        Link<T>*tmp;
        while(head!=NULL){
            tmp=head;
            head=head->next;
            delete tmp;
        }
    }
    bool empty(){
        if(head->next!=NULL)
            return false;
        else
            return true;    
    }
    void clear(){
        Link<T>*tmp;
        while(head!=NULL){//头结点也要删掉
            tmp=head;
            head=head->next;
            delete tmp;
        }
        head=tail=new Link<T>;
    }
    int length(){
        int cnt=0;
        Link<T>*tmp=head;
        while(tmp->next!=NULL){
            tmp=tmp->next;
            cnt++;
        }
        return cnt;
    }
    bool append(const T value){
        // Link<T> add(value);
        // tail->next=&add;
        // tail=&add;
        Link<T>*p=new Link<T>(value);//p.next=NULL
        tail->next=p;
        tail=p;
        return true;
    }
    bool insert(const int i,const T value){//插入数据内容为value的结点作为第i个结点
        Link<T>*p,*q;
        if((p=setPos(i-1))==NULL){//没有前驱
            cout<<"非法插入点"<<endl;
            return false;
        } 
        else{
            q=new Link<T>(value,p->next);//此时p是前驱,新结点指向右边结点
            p->next=q;
            if(p==tail)//相当于append
                tail=q;
            return true;
        }  
    }
    bool myDelete(const int i){
        Link<T>*p=setPos(i),*q=setPos(i-1);
        if(p==NULL){
            cout<<"非法删除点"<<endl;
            return false;
        }
        else{
            q->next=p->next;
            if(tail==p){
                tail=q;
            }
            delete(p);
            return true;
        }
    }
    bool getValue(const int i,T&value){
        Link<T>*p=setPos(i);
        if(p==NULL){
            cout<<"非法查找点"<<endl;
            value=-1;
            return false;
        }
        else{
            value=p->data;
            return true;
        }
    }
    bool getPos(int&i,const T value){
        i=-1;
        Link<T>*p=head;
        int cnt=-1;
        while(p->next!=NULL){
            p=p->next;
            cnt++;
            if(p->data==value){
                i=cnt;
                return true;
            }
        }
        cout<<"链表中不存在该值"<<endl;
        return false;
    }
    void print(){
        Link<T>*p=head;
        while(p->next!=NULL){
            p=p->next;
            cout<<p->data<<' ';
        }
        cout<<endl;
    }
};

void arrListTest(){//顺序表测试
    arrList<int> myArrList(10);
    myArrList.print();//初始状态检查
    for(int i=0;i<6;i++)
        myArrList.append(i);//添加元素
    myArrList.print();
    myArrList.clear();//清空
    myArrList.print();
    for(int i=0;i<6;i++)
        myArrList.append(i);//添加元素
    myArrList.print();cout<<myArrList.length()<<endl;
    myArrList.insert(8,10);//错误插入元素
    myArrList.print();
    myArrList.insert(0,5);//正确插入元素
    myArrList.print();
    myArrList.myDelete(0);//正确删除元素
    myArrList.print();
    myArrList.myDelete(6);//错误删除元素
    myArrList.setValue(0,5);//设置值
    myArrList.print();
    int posTest;
    int valueTest;
    if(myArrList.getValue(0,valueTest))//返回元素值
    cout<<"valueTest="<<valueTest<<endl;
    if(myArrList.getValue(-1,valueTest))//错误返回元素值
    cout<<"valueTest="<<valueTest<<endl;
    if(myArrList.getPos(posTest,2))//查找元素值
    cout<<"posTest="<<posTest<<endl;
    if(myArrList.getPos(posTest,8))//错误查找元素值
    cout<<"posTest="<<posTest<<endl;
}

void lnkListTest(){//单向链表测试
    lnkList<int> myLnkList;
    cout<<myLnkList.empty()<<endl;
    myLnkList.print();
    for(int i=0;i<6;i++)
        myLnkList.append(i);
    myLnkList.print();
    cout<<myLnkList.length()<<endl;
    myLnkList.insert(10,6);
    myLnkList.print();
    myLnkList.insert(6,6);
    myLnkList.print();
    cout<<myLnkList.length()<<endl;
    myLnkList.clear();
    myLnkList.print();
    cout<<myLnkList.length()<<endl;
    for(int i=0;i<6;i++)
        myLnkList.insert(i,i);
    myLnkList.print();
    cout<<myLnkList.length()<<endl;
    myLnkList.myDelete(10);
    myLnkList.myDelete(0);
    myLnkList.print();
    cout<<myLnkList.length()<<endl;
    int getValueTest,getPosTest;
    myLnkList.getValue(10,getValueTest);
    myLnkList.getValue(0,getValueTest);
    cout<<getValueTest<<endl;
    myLnkList.getPos(getPosTest,10);
    myLnkList.getPos(getPosTest,3);
    cout<<getPosTest<<endl;
}
int main(){
    // arrListTest();
    lnkListTest();
    return 0;
}