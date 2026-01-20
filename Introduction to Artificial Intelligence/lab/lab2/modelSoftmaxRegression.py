'''
Softmax 回归。计算accuracy。
'''
import math
from copy import deepcopy
from typing import List
from autograd.BaseGraph import Graph
from autograd.utils import buildgraph
from autograd.BaseNode import *
import mnist
from answerSoftmaxRegression import buildGraph, lr, wd1, wd2
import pickle
from util import setseed

setseed(0) # 固定随机数种子以提高可复现性

save_path = "model/sr.npy"


X=mnist.trn_X
Y=mnist.trn_Y


if __name__ == "__main__":
    graph = buildGraph(Y)
    # 训练
    best_train_acc = 0
    graph.train()
    for i in range(1, 60+1):
        graph.flush()
        pred, loss = graph.forward(X)[-2:]
        haty = np.argmax(pred, axis=1)
        graph.backward()
        graph.optimstep(lr, wd1, wd2)
        acc = np.average(haty==mnist.trn_Y)
        print(f"epoch {i} loss {loss:.3e} acc {acc:.4f}")
        if acc > best_train_acc:
            best_train_acc = acc
            with open(save_path, "wb") as f:
                pickle.dump(graph, f)

    # 测试
    with open(save_path, "rb") as f:
        graph = pickle.load(f)
    graph.eval()
    graph.flush()
    pred = graph.forward(mnist.val_X, removelossnode=1)[-1]
    haty = np.argmax(pred, axis=1)
    print("valid acc", np.average(haty==mnist.val_Y))

