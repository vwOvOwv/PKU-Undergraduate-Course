import numpy as np
from copy import deepcopy
from typing import List, Callable

EPS = 1e-6

# 超参数，分别为树的最大深度、熵的阈值、信息增益函数
# TODO: You can change or add the hyperparameters here
hyperparams = {"depth":100, "purity_bound":1e-2, "gainfunc":"gainratio"}

def entropy(Y: np.ndarray):
    """
    计算熵
    @param Y: n, 标签向量
    @return: 熵
    """
    # TODO: YOUR CODE HERE
    h=0
    n=Y.shape[0]
    values,valuesCnt=np.unique(Y,return_counts=True)
    valuesP=valuesCnt/n
    valuesLogP=np.log2(valuesP)
    m=values.shape[0]
    for i in range(m):
        h+=valuesP[i]*valuesLogP[i]
    h*=-1
    return h
    raise NotImplementedError


def gain(X: np.ndarray, Y: np.ndarray, idx: int):
    """
    计算信息增益
    @param X: n*d, 每行是一个输入样本。 n: 样本数量， d: 样本的维度
    @param Y: n, 样本的label
    @param idx: 第idx个特征
    @return: 信息增益
    """
    feat = X[:, idx]
    ufeat, featcnt = np.unique(feat, return_counts=True)
    featp = featcnt / feat.shape[0]
    ret = 0
    # TODO: YOUR CODE HERE
    h_D=entropy(Y)
    for i in range(ufeat.shape[0]):
        Y_=[]
        for j in range(feat.shape[0]):
            if feat[j]==ufeat[i]:
                Y_.append(Y[j])
        Y_=np.array(Y_)
        ret+=featp[i]*entropy(Y_)
    ret=h_D-ret
    # raise NotImplementedError
    return ret


def gainratio(X: np.ndarray, Y: np.ndarray, idx: int):
    """
    计算信息增益比
    @param X: n*d, 每行是一个输入样本。 n: 样本数量， d: 样本的维度
    @param Y: n, 样本的label
    @param idx: 第idx个特征
    @return: 信息增益比
    """
    ret = gain(X, Y, idx) / (entropy(X[:, idx]) + EPS)
    return ret


def giniD(Y: np.ndarray):
    """
    计算基尼指数
    @param Y: n, 样本的label
    @return: 基尼指数
    """
    u, cnt = np.unique(Y, return_counts=True)
    p = cnt / Y.shape[0]
    return 1 - np.sum(np.multiply(p, p))


def negginiDA(X: np.ndarray, Y: np.ndarray, idx: int):
    """
    计算负的基尼指数增益
    @param X: n*d, 每行是一个输入样本。 n: 样本数量， d: 样本的维度
    @param Y: n, 样本的label
    @param idx: 第idx个特征
    @return: 负的基尼指数增益
    """
    feat = X[:, idx]
    ufeat, featcnt = np.unique(feat, return_counts=True)
    featp = featcnt / feat.shape[0]
    ret = 0
    for i, u in enumerate(ufeat):
        mask = (feat == u)
        ret -= featp[i] * giniD(Y[mask])
    ret += giniD(Y)  # 调整为正值，便于比较
    return ret


class Node:
    """
    决策树中使用的节点类
    """
    def __init__(self): 
        self.children = {}          # 子节点
        self.featidx: int = None    # 用于划分的特征
        self.label: int = None      # 叶节点的标签

    def isLeaf(self):
        """
        判断是否为叶节点
        @return:
        """
        return len(self.children) == 0


def buildTree(X: np.ndarray, Y: np.ndarray, unused: List[int], depth: int, purity_bound: float, gainfunc: Callable, prefixstr=""):#加入purity_bound剪枝
    root = Node()
    u, ucnt = np.unique(Y, return_counts=True)
    root.label = u[np.argmax(ucnt)]#取多数类
    # print(prefixstr, f"label {root.label} numbers {u} count {ucnt} depth {depth}") #可用于debug
    # 当达到终止条件时，返回叶节点
    # TODO: YOUR CODE HERE
    # if unused==[] or X.shape[0]==1 or depth>hyperparams["depth"] or purity_bound<hyperparams["purity_bound"]:
    if unused==[] or X.shape[0]==1 or depth<=0 or entropy(Y)<purity_bound:
        return root
    # raise NotImplementedError
    gains = [gainfunc(X, Y, i) for i in unused]
    idx = np.argmax(gains)
    root.featidx = unused[idx]
    unused = deepcopy(unused)
    unused.pop(idx)
    feat = X[:, root.featidx]
    ufeat = np.unique(feat)
    # 按选择的属性划分样本集，递归构建决策树
    # 提示：可以使用prefixstr来打印决策树的结构
    # TODO: YOUR CODE HERE
    for id in ufeat:
        X_=[]
        Y_=[]
        for i in range(X.shape[0]):
            if X[i][root.featidx]==id:
                X_.append(X[i])
                Y_.append(Y[i])
        X_=np.array(X_)
        Y_=np.array(Y_)
        root.children[id]=buildTree(X_,Y_,unused,depth-1,purity_bound,gainfunc,prefixstr)
    # raise NotImplementedError
    return root


def inferTree(root: Node, x: np.ndarray):
    """
    利用建好的决策树预测输入样本为哪个数字
    @param root: 当前推理节点
    @param x: d*1 单个输入样本
    @return: int 输入样本的预测值
    """
    if root.isLeaf():
        return root.label
    child = root.children.get(x[root.featidx], None)
    return root.label if child is None else inferTree(child, x)

