
template <class Key, class Elem, class KEComp, class EEComp>
class hashdict 
 {
private:
  Elem* HT;     // 散列表
  int M;       // 散列表大小
   Elem TOMB;    //墓碑
  int currcnt;  // 现有元素数目
  Elem EMPTY;  // 空槽
  int p(Key K, int i) const // 探查函数
    { return linear(i); }
  int h(int x) const { return x % M; } // 散列函数
  int h(char* x) const { // 字符串散列函数
    int i, sum;
    for (sum=0, i=0; x[i] != '\0'; i++) sum += (int) x[i];
    return(sum % M);
  }
  int linear(int i) const {return i;}        //线性探查
  int square(int i) const {                  //二次探查
	  if (i%2==0) return -(i*i/4);
      else       return (i+1)*(i+1)/4;
  }
  int rehash (Key K, int i) const;
  int division(int x ) const { return  x % M; }  //除余法
  int ELFhash ( char*  key )const {
              unsigned long h = 0;
			  while(*key) { 
                      h = ( h << 4 ) + *key++;
                      unsigned long g = h&0xF0000000L;
                      if (g) h^= g >> 24;
                      h  = ~g;
                        }
                  return h % M;
                   }

public:
   hashdict(int sz, Elem e, Elem t){// 构造函数, e用来定义空槽
          M=sz; EMPTY= e; TOMB=t;
          currcnt=0; HT=new Elem[sz];
          for(int i=0; i<M; i++) HT[i]= EMPTY; 
}
  ~hashdict() { delete HT; }
  bool hashInsert(const Elem&);
  bool hashSearch(const Key&, Elem&) const;
  Elem hashDelete(const Key& K);
  int size() { return currcnt; } // 散列表中现有元素数
};
