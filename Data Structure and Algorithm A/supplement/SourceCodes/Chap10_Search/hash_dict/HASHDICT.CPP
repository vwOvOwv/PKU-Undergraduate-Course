#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "Int.h"
#include "../COMPARE.H"

#include "HASHDICT.H"

// 检索关键码值为k的记录。假定每个关键码的探查序列中
//至少有一个槽是空的，否则它们就会进入一个无限循环中。

template <class Key, class Elem, class KEComp, class EEComp>
bool hashdict<Key, Elem, KEComp, EEComp>::hashSearch(const Key& K, Elem& e) const {
  int home= h(K);                    // home 保存K的基地址
  int i=0;                           // 
  int pos = home;                    // 探查序列的初始位置

  while(!EEComp::eq(EMPTY, HT[pos])){

	  if (KEComp::eq(K, HT[pos])) {  // 发现目标
          e = HT[pos];
         return true;
	  }
      i++;
	  pos = (home + p(K, i)) % M;

  }
   return false;    
}

   int getkey(Int* e) 
   { 
	   return e->key();
   }
   
   
   char* getkey(char* e)
   { 
	   return e;
   }

// 将数据元素e插入到散列表 HT

template <class Key, class Elem, class KEComp, class EEComp>
bool hashdict<Key, Elem, KEComp, EEComp>::
hashInsert(const Elem& e) {
  int home;                                      //home存储基位置      
  int i=0;     
  int pos = home= h(getkey(e));                 //初始化探查序列
  while(!EEComp::eq(EMPTY, HT[pos])){
     if (EEComp::eq(e, HT[pos])) 
		 return false; // 发现重复, 返回
     i++;
     pos = (home + p(getkey(e), i)) % M;      // 继续向前探查

  }
    HT[pos] = e;                             // 插入元素e
    return true;
}
 


 

int Intkey(Int& e) { return e.key(); }
char* charkey(char*& e) { return e; }

int main(int argc, char** argv) {
  hashdict<int, Int*, intIntsCompare, IntsIntsCompare> dict(100, new Int(-1));
  Int* val;

  dict.hashInsert(new Int(10));
  if (dict.hashSearch(10, val))
    cout << "发现值 " << val << " 匹配关键码10\n";
  else
    cout << "没有发现关键码为10的元素\n";

  hashdict<char*, char*, CCCompare, CCCompare> Strdict(100, "");
  char* str;

  Strdict.hashInsert("hello");
  if (Strdict.hashSearch("hello", str))
    cout << "发现值  " << str << " 匹配关键码 hello\n";
  else
    cout << "没有发现关键码为hello的元素\n";
  return 0;
}
