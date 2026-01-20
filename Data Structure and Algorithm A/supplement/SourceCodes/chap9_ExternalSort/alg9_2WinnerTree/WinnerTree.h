//****************   WinnerTree.h   *****************//
#define MAX 8		//选手数

template<class T>
class WinnerTree  {
public:
	WinnerTree(int TreeSize = MAX);
	~WinnerTree() {delete [] B;} 
	void Initialize(T A[], int size, int(*winner)(T A[], int b, int c));  //初始化赢者树
	int Winner()  const {return (n) ? B[1] : 0;}     ////返回最终胜者的索引，在赢者树中这个索引放在B[1]中
	int Winner(int i) const {return (i < n) ? B[i] : 0;}
	void RePlay(int i, int(*winner) (T A[], int b, int c)); //位置i的外部选手改变后重构赢者树
	void Output() const;
private:
	void Play(int p, int lc, int rc, int(*winner)(T A[], int b, int c));

	int MaxSize;	 //允许的最大选手数
	int n;			 //当前选手数
	int LowExt;		 //最底层外部结点数
	int offset;	     //最底层外部结点之上的结点总数
	int *B;          //赢者树数组，实际存放的是下标
	T *L;            //元素数组
};

//构造函数
template<class T>
WinnerTree<T>::WinnerTree(int TreeSize)  {
	MaxSize = TreeSize;
	B = new int[MaxSize];
	n = 0;
}

//初始化赢者树
template<class T>
void WinnerTree<T>::Initialize(T A[], int size, int(*winner)(T A[], int b, int c))  {
	if (size > MaxSize || size < 2)   //能否处理size个选手的数组A[]
		return; 
	n = size;                   //初始化成员变量
	L = A; 
	int i, s;
	for (s = 1; 2*s <= n-1; s += s);  //计算 s = 2^log(n-1)
	LowExt = 2*(n - s);
	offset = 2*s - 1; 
	for (i = 2; i <= LowExt; i += 2)     //最底层外部结点的比赛
		Play((offset+i)/2, i-1, i, winner); 
//处理其余外部结点
//若n为奇数，内部结点和外部结点比赛。这里用L[LowExt+1]和它的父结点比赛
	if (n % 2)  {
		Play(n/2, B[n-1], LowExt+1, winner);
		i = LowExt + 3;
	}
	else 
		i = LowExt + 2;
	for (; i <= n; i += 2)        //剩余外部结点的比赛
		Play((i-LowExt+n-1)/2, i-1, i, winner);
}


//从内部结点B[p]处开始比赛
template<class T>
void WinnerTree<T>::Play(int p, int lc, int rc, int(*winner)(T A[], int b, int c))  {
	B[p] = winner(L, lc, rc); 		//胜者索引放在B[p]中
	while (p > 1 && p % 2)   {     //右孩子，需要沿路径继续向上比赛
		B[p/2] = winner(L, B[p-1], B[p]);
		p /= 2;  //父结点
	}
}


//成员函数RePlay负责选手i的值改变后重新开始比赛
template<class T>
void WinnerTree<T>::RePlay(int i, int(*winner)(T A[], int b, int c))  {
	if (i <= 0 || i > n) 
		return; 
int p, lc, 	rc;     	//比赛结点极其左右孩子的下标
if (i <= LowExt)  {	    //从最底层开始
		p = (offset  +  i)/2;
		lc = 2*p - offset; // left child of p
		rc = lc+1;
	}
	else  {
		p = (i-LowExt+n-1)/2;
		if (2*p == n-1)  {
			lc = B[2*p];
			rc = i;
		}
		else  {
			lc = 2*p - n  +  1  +  LowExt;
			rc = lc+1;
		}
	} 
	B[p] = winner(L, lc, rc); 	 //B[p]中保存赢者的索引
	//完成剩下的比赛
	p /= 2;
	for (; p >= 1; p /= 2)       //沿路径向上比赛
		B[p] = winner(L, B[2*p], B[2*p+1]);
}


//输出函数
template<class T>
void WinnerTree<T>::Output() const  {
	cout << "当前选手数(size) = "<< n << " 最底层外部结点数(LowExt) = " << LowExt
         << " 最底层外部结点之上的结点总数(Offset) = " << offset << endl;
	cout << "赢者树B:" << endl;
	for (int i = 1; i < n; i++)
		cout << B[i] << ' ';
	cout << endl << endl;
}

//返回赢者
int winner(int A[], int b, int c)  {
	if (A[b] <= A[c])
		return b;
	return c;
}