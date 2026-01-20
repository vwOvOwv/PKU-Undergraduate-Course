//****************************  genSplay.h  ****************************
//伸展树类模版

#ifndef SPLAYING
#define SPLAYING
using namespace std;
template<class T> class SplayTree;

template<class T>
class SplayingNode 
{//伸展树的结点
public:
	SplayingNode *left, *right, *parent;//结点的左右儿子和父亲结点
    T info;                     //存储的数据
	SplayingNode() 
	{//构造函数
        left = right = parent = 0;
    }
    
	SplayingNode(const T& el, SplayingNode *l = 0, SplayingNode *r = 0,
                 SplayingNode *p = 0) 
	{//构造函数
        info = el; left = l; right = r; parent = p;
    }
};

template<class T>
class SplayTree 
{//伸展树类
public:
    SplayTree()
	{
        root = 0;
    }
    void inorder() 
	{//排序
        inorder(root);
    }
    T* search(const T&);//搜索元素
    void insert(const T&);//插入元素
protected:
    SplayingNode<T> *root; //根结点
    void rotateR(SplayingNode<T>*);  //右旋转
    void rotateL(SplayingNode<T>*);  //左旋转
    void continueRotation(SplayingNode<T>* gr, SplayingNode<T>* par,
                          SplayingNode<T>* ch, SplayingNode<T>* desc);
	//进行“之“型或者“一“型旋转
    void splay(SplayingNode<T>*); 
    void inorder(SplayingNode<T>*);
    void virtual visit(SplayingNode<T>*) { }
};


template<class T>
void SplayTree<T>::continueRotation(SplayingNode<T>* gr, SplayingNode<T>* par,
                                    SplayingNode<T>* ch, SplayingNode<T>* desc) 
{
    if (gr != 0) { // 结点是否有祖父结点
         if (gr->right == ch->parent)
              gr->right = ch;
         else gr->left  = ch;
    }
    else root = ch;
    if (desc != 0)
        desc->parent = par;
    par->parent = ch;
    ch->parent = gr;
}

template<class T>
void SplayTree<T>::rotateR(SplayingNode<T>* p) 
{
    p->parent->left = p->right;
    p->right = p->parent;
    continueRotation(p->parent->parent,p->right,p,p->right->left);
}

template<class T>
void SplayTree<T>::rotateL(SplayingNode<T>* p) 
{
    p->parent->right = p->left;
    p->left = p->parent;
    continueRotation(p->parent->parent,p->left,p,p->left->right);
}

template<class T>
void SplayTree<T>::splay(SplayingNode<T>* p)
{
    while (p != root) {
        if (p->parent->parent == 0)    // 如果结点的父亲是根结点
             if (p->parent->left == p)
                  rotateR(p);
             else rotateL(p);
        else if (p->parent->left == p) // 如果结点是左子女
             if (p->parent->parent->left == p->parent) {
                  rotateR(p->parent);
                  rotateR(p);
             }
             else {
                  rotateR(p); // 旋转p和它的父亲
                  rotateL(p); // 旋转p和他的新父亲
             }
        else                          // 如果结点是右子女
             if (p->parent->parent->right == p->parent) {
                  rotateL(p->parent);
                  rotateL(p);
             }
             else {
                  rotateL(p); // 旋转p和它的父亲
                  rotateR(p); // 旋转p和他的新父亲
             }
        if (root == 0)              // 更新根结点
            root = p;
    }
}

template<class T>
T* SplayTree<T>::search(const T& el) 
{
    SplayingNode<T> *p = root;
    while (p != 0)
        if (p->info == el) {        // el元素是否在树中
             splay(p);          
             return &p->info;
        }
        else if (el < p->info)
             p = p->left;
        else p = p->right;
    return 0;
}

template<class T>
void SplayTree<T>::insert(const T& el) {
    SplayingNode<T> *p = root, *prev = 0, *newNode;
    while (p != 0) {  // 插入一个新的元素
        prev = p;
        if (el < p->info)
             p = p->left;
        else p = p->right;
    }
    if ((newNode = new SplayingNode<T>(el,0,0,prev)) == 0) {
         cerr << "No room for new nodes\n";
         exit(1);
    }
    if (root == 0)    // 树是空的
         root = newNode;
    else if (el < prev->info)
         prev->left  = newNode;
    else prev->right = newNode;
}

template<class T>
void SplayTree<T>::inorder(SplayingNode<T> *p) 
{
     if (p != 0) {
         inorder(p->left);
         visit(p);
         inorder(p->right);
     }
}

#endif
