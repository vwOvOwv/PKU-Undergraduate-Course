//**************   LooserTree.h   ***************//

// 函数功能：返回赢者
int winner(int A[], int b, int c)  {
	return A[b]<=A[c] ? b:c;
}

// 函数功能：返回败者
int loser(int A[], int b, int c)  {
	return A[b]>A[c] ? b:c;
}

// class
// -----------------------------------------------------------------------------------------------
template <class T>
class LoserTree  {
public:
	LoserTree(int Treesize);
	~LoserTree() {delete [] B;}; 
	void Initialize(T A[], int size, int(*winner)(T A[], int b, int c), int(*loser)(T A[], int b, int c));
	int Winner();
	void RePlay(int i,  int(*winner)(T A[], int b, int c), int(*loser)(T A[], int b, int c));
	void Output() const;
	
private:
	void Play(int p, int lc, int rc,  int(*winner)(T A[], int b, int c), int(*loser)(T A[], int b, int c));
					// 在内部结点从右分支向上比赛
	int MaxSize; // 最大选手数
	int n;   // 当前选手数
	int LowExt;  // 最底层外部结点数 lowest-level external nodes
	int offset;  // 最底层外部结点之上的结点数 2^k - 1
	int *B;   // 赢者数数组 array for winner tree
	T *L;   // 选手数组 element array
};

//构造函数
template<class T>
LoserTree<T>::LoserTree(int TreeSize)  {
	MaxSize = TreeSize;
	B = new int[MaxSize];
	n = 0;
}

//成员函数Winner，返回最终胜者的索引，在败者树中这个索引存放在B[0]中
template<class T>
int LoserTree<T>::Winner()  {  // 返回最终胜者的索引B[0]
	return (n)?B[0]:0;
}

//成员函数Initilalize负责初始化败者树
template<class T>
void LoserTree<T>::Initialize(T A[], int size, int(*winner)(T A[], int b, int c),
							  int(*loser)(T A[], int b, int c))   {// 初始化败者树
	if(size > MaxSize || size < 2)  {        //能否处理size个选手的数组a[]
		cout<<"Bad Input!"<<endl<<endl;
		return;
	}	
	n = size;                    //初始化成员变量
	L = A;	
	int i,s;
	for(s = 1; 2*s <= n-1; s += s);    //计算s = 2^log(n-1)
	LowExt = 2*(n-s);
	offset = 2*s-1;
	//最底层外部结点的比赛
	for(i = 2; i <= LowExt; i += 2)     // 最底层外部结点的比赛
		Play((offset+i)/2, i-1, i, winner, loser);
	//处理其余外部结点
	if(n%2)  {           // n为奇数，内部结点和外部结点比一次
		Play(n/2, B[(n-1)/2], LowExt+1, winner, loser);//暂存在父结点的左胜者与外部右子结点比
		i = LowExt+3;
	}
	else
		i = LowExt+2;	
	for(; i <= n; i += 2)   // 剩余外部结点的比赛
		Play((i-LowExt+n-1)/2, i-1, i, winner, loser);  
}

//成员函数Play负责在内部结点B[p]处开始比赛
template<class T>
void LoserTree<T>::Play(int p, int lc, int rc, int(* winner)(T A[],
						int b, int c), int(* loser)(T A[], int b, int c))  {  // 在内部结点从右分支向上比赛
	B[p] = loser(L, lc, rc);       //败者索引放在B[p]中			
	int temp1, temp2;
	temp1 = winner(L, lc, rc);    //p处的胜者索引
	while(p>1 && p%2)  {       // p为奇数右孩子，沿路径继续向上比赛
		temp2 = winner(L, temp1, B[p/2]);
		//p的胜者和p的父结点比较，赢者暂存在temp2中
		B[p/2] = loser(L, temp1, B[p/2]);                //败者索引放入B[p/2]
		temp1 = temp2;         // 胜者放入temp1
		p/=2;
	}//while
	B[p/2] = temp1;             // B[p]是左孩子或者p=1，结束
	
}

//成员函数RePlay负责选手i的值改变后重新开始比赛
template<class T>
void LoserTree<T>::RePlay(int i, int (*winner)(T A[], int b, int c), 
						  int (*loser)(T A[], int b, int c))  {  // 选手i的值改变后，重新开始比赛
	int p;               // 用于计算父结点索引的临时变量
	if(i <= 0 || i > n)  {
		cout<<"Out of Bounds!"<<endl<<endl;
		return;
	}
	//确定父结点的位置
	if (i <= LowExt)
		p = (i+offset)/2;
	else
		p = (i-LowExt+n-1)/2;
	B[0] = winner(L, i, B[p]);         //B[0]中始终保存胜者的索引
	B[p] = loser(L, i, B[p]); 	      //B[p]中保存败者的索引
	for(; (p/2) >= 1; p/=2)  { 	      //沿路径向上比赛
		int temp;//临时存放赢者的索引
		temp = winner(L,B[p/2], B[0]);
		B[p/2] = loser(L,B[p/2], B[0]);
		B[0] = temp;
	}
}


// 函数功能：输出
template<class T>
void LoserTree<T>::Output() const  {
	cout << "当前选手数(size) = "<< n << " 最底层外部结点数(LowExt) = " << LowExt
         << " 最底层外部结点之上的结点总数(Offset) = " << offset << endl;
	cout << "败者树B（B0存放胜者下标）: " << endl;
	for (int i = 0; i < n; i++)
		cout << B[i] << ' ';
	cout << endl << endl;
}