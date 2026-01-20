#include <string>
#include <iostream>
//using namespace std;


template<size_t N> 
class mySet {
	typedef unsigned long ulong;
private:
	enum {
		NB = 8 * sizeof (ulong),			//unsigned long数据类型的位的数目
		LI = N == 0 ? 0 : (N - 1) / NB		//数组最后一个元素的下标
	};

	ulong A[LI + 1];						//存放位向量的数组
	void Tidy(ulong X = 0);					//将数组中的每个元素设成X
	void Trim();							//将不使用的位设成0

public:	
	mySet() ;								//构造函数
	mySet(ulong X);
	
	mySet<N>& set();						//设置元素属性
	mySet<N>& set(size_t P, bool X = true);
	mySet<N>& reset();						//把集合设为空
	mySet<N>& reset(size_t P);				//删除元素P
	
	bool at(size_t P) const;				//属于运算
	bool operator[](size_t P) const ;

	mySet<N>& flip();						//求反
	mySet<N>& flip(size_t P);				
	ulong to_ulong() const;
	std::string to_string() const;				
	size_t count() const;					//计算集合内元素数目
	size_t size() const;

	bool test(size_t P) const;				
	bool any() const;						//判断集合是否包含元素
	bool none() const;						//判断集合是否为空

	mySet<N> operator~() const;					//非运算
	mySet<N>& operator&=(const mySet<N>& R);	//交等于
	mySet<N>& operator|=(const mySet<N>& R);	//并等于
	mySet<N>& operator^=(const mySet<N>& R);	//异或等于

	bool operator==(const mySet<N>& R) const;	//等于
	bool operator!=(const mySet<N>& R) const;	//不等
	bool operator<=(const mySet<N>& R) const;	//包含于
	bool operator< (const mySet<N>& R) const;	//真包含于
	bool operator>=(const mySet<N>& R) const;	//包含
	bool operator>(const mySet<N>& R) const;	//真包含

	friend mySet<N> operator&(const mySet<N>& L, const mySet<N>& R){  //交运算
		return (mySet<N>(L) &= R); 
	};
	friend mySet<N> operator|(const mySet<N>& L, const mySet<N>& R){  //并运算
		return (mySet<N>(L) |= R); 
	};
	friend mySet<N> operator-(const mySet<N>& L, const mySet<N>& R){  //差运算
		return (mySet<N>(L) & ~R); 
	};
	friend mySet<N> operator^(const mySet<N>& L, const mySet<N>& R){  //异或运算
		return (mySet<N>(L) ^= R); 
	};
	friend std::ostream& operator<<(std::ostream& O, const mySet<N>& R){        //重载<<运算符
		for (size_t P = 0; P < N; P++ )
			O << (R.test(P) ? '1' : '0');
		return (O); 
	};

	};

template<size_t N>  void mySet<N>::Tidy(ulong X){   //将数组中的每个元素设成X
	for (int I = LI; I >= 0; I--)
			A[I] = X;
	if (X != 0)
			Trim(); 
}

template<size_t N>  void mySet<N>::Trim(){			//将不使用的位设成0
	if (N % NB != 0)
			A[LI] &= ((ulong)1 << N % NB) - 1;
}

template<size_t N> 	mySet<N>::mySet(){				//构造函数
	Tidy();
}

template<size_t N> 	mySet<N>::mySet(ulong X){
	Tidy();
	for (size_t P = 0; X != 0 && P < N; X >>= 1, ++P)
		if (X & 1)
			set(P); 
}

template<size_t N> mySet<N>& mySet<N>::set(){		//设置元素属性
	Tidy(~(ulong)0);
	return (*this); 
}

/*
template<size_t N>	mySet<N>& mySet<N>::set(size_t P){
	A[P / NB] |= (ulong)1 << P % NB;
	return (*this); 
}
*/


template<size_t N>	mySet<N>& mySet<N>::set(size_t P, bool X){
	if (X)   									// X为真，位向量中相应值设为1
		A[P / NB] |= (ulong)1 << (P % NB); 		// P对应的元素进行按位或运算
	else									
		A[P / NB] &= ~((ulong)1 << (P % NB));	// X为假，位向量中相应值设为0
	return (*this); 
}

template<size_t N> mySet<N>& mySet<N>::reset(){				//把集合设置为空
	Tidy();
	return (*this); 
}

template<size_t N> mySet<N>& mySet<N>::reset(size_t P){		//把具体元素设置为空
	return set(P, 0);
}

template<size_t N> bool mySet<N>::at(size_t P) const{		//属于运算
	return (test(P)); 
}
	
template<size_t N> bool mySet<N>::operator[](size_t P) const {
	return (test(P)); 
}

template<size_t N> mySet<N>& mySet<N>::flip(){				//求非运算
	for (int I = LI; I >= 0; I--)
		A[I] = ~A[I];
	Trim();
	return (*this); 
}

template<size_t N> mySet<N>& mySet<N>::flip(size_t P){      // 取反
	A[P / NB] ^= (ulong)1 << P % NB;
	return (*this); 
}

template<size_t N> unsigned long mySet<N>::to_ulong() const{ // 转换为ulong类型
	return A[0]; 
}

template<size_t N> std::string mySet<N>::to_string() const{   // 用0，1形式把集合内容输出到字符串
	std::string S;
	S.reserve(N);
	for (size_t P = N; 0 < P; )
		S += test(--P) ? '1' : '0';
	return (S); 
}

template<size_t N> size_t mySet<N>::count() const{      //计算集合元素数目
	size_t V = 0;
	for (int I = LI; I >= 0; I--)
		for (ulong X = A[I]; X != 0; X >>= 4)
			V += "\0\1\1\2\1\2\2\3"						//"\0\1\1\2\1\2\2\3\1\2\2\3\2\3\3\4"代表十六进制数0-F所包含1的数目的数组
				"\1\2\2\3\2\3\3\4"[X & 0xF];			
	return (V); 
}

template<size_t N> size_t mySet<N>::size() const{        
	return (N); 
}
	
template<size_t N> bool mySet<N>::test(size_t P) const{   
	return ((A[P / NB] & ((ulong)1 << P % NB)) != 0); 
}

template<size_t N> bool mySet<N>::any() const{           //判断集合是否包含元素
	for (int I = LI; I >= 0; I--)
		if (A[I] != 0)
			return (true);
	return (false); }

template<size_t N> bool mySet<N>::none() const{          //判断集合是否为空
	return (!any()); 
}

template<size_t N> mySet<N> mySet<N>::operator~() const{	//非运算
	return (mySet<N>(*this).flip()); 
}

template<size_t N>	mySet<N>& mySet<N>::operator&=(const mySet<N>& R) { //赋值交
	for (int I = LI; I >= 0; I--)			// 从低位到高位
		A[I] &= R.A[I];						// 以ulong元素为单位按位交
	return (*this); 
}

template<size_t N>	mySet<N>& mySet<N>::operator|=(const mySet<N>& R) { //并等于运算
	for (int I = LI; I >= 0; I--)
		A[I] |= R.A[I];
	return (*this); 
}

template<size_t N>	mySet<N>& mySet<N>::operator^=(const mySet<N>& R) {  //异或等于运算
	for (int I = LI; I >= 0; I--)
		A[I] ^= R.A[I];
	return (*this); 
}
	
template<size_t N>	bool mySet<N>::operator==(const mySet<N>& R) const{  //等于运算
	for (int I = LI; I >= 0; I--)
		if (A[I] != R.A[I])
			return (false);
	return (true); }

template<size_t N>	bool mySet<N>::operator!=(const mySet<N>& R) const{  //不等运算
	return (!(*this == R)); 
}
	
template<size_t N>	bool mySet<N>::operator<=(const mySet<N>& R) const{  //包含于运算
	return ((mySet(*this) |= R) == R);
}

template<size_t N>	bool mySet<N>::operator<(const mySet<N>& R) const{   //真包含运算
	 return ((*this <= R) && (*this != R));
}

template<size_t N>	bool mySet<N>::operator>=(const mySet<N>& R) const{  //包含运算
	return ((mySet(*this) &= R) == R);
}

template<size_t N>	bool mySet<N>::operator>(const mySet<N>& R) const{   //真包含运算
	return ((*this >= R) && (*this != R));	
}

/*  &运算只是friend, LI属于未定义
template<size_t N>	 mySet<N> operator&( const mySet<N>& L,  const mySet<N>& R) {//交运算
	mySet<N> s = L;
	for (int I = LI; I >= 0; I--)
		s.A[I] &= R.A[I];
	return s; 	
}
*/