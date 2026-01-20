import mnist
from copy import deepcopy
from typing import List
from autograd.BaseGraph import Graph
from autograd.utils import buildgraph
from autograd.BaseNode import *

# 超参数
# TODO: You can change the hyperparameters here
lr = 1e-5   # 学习率
wd1 = 1e-5  # L1正则化
wd2 = 1e-5  # L2正则化
batchsize = 128

def buildGraph(Y):
    """
    建图
    @param Y: n 样本的label
    @return: Graph类的实例, 建好的图
    """
    # TODO: YOUR CODE HERE
    nodes=[]
    nodes.append(Linear(784,700))
    nodes.append(Dropout())
    nodes.append(Linear(700,600))
    nodes.append(relu())
    nodes.append(Linear(600,500))
    nodes.append(Dropout())
    nodes.append(Linear(500,400))
    nodes.append(relu())
    nodes.append(Linear(400,300))
    nodes.append(Dropout())
    nodes.append(Linear(300,200))
    nodes.append(Dropout())
    nodes.append(Linear(200,100))
    nodes.append(Dropout())
    nodes.append(Linear(100,10))
    nodes.append(Dropout())
    nodes.append(Linear(10,10))
    nodes.append(Softmax())
    nodes.append(CrossEntropyLoss(Y))
    return Graph(nodes)
    return None
