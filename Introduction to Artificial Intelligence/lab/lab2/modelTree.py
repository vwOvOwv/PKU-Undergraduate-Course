import numpy as np
# you only need matplotlib if you want to create some plots of the data
import matplotlib.pyplot as plt
import mnist
from answerTree import *
import pickle
from util import setseed

setseed(0) # 固定随机数种子以提高可复现性

save_path = "model/tree.npy"

def discretize(x, bin=64):
    return np.divmod(x, bin)[0]

trn_X = discretize(mnist.trn_X)
trn_Y = mnist.trn_Y
val_X = discretize(mnist.val_X)
val_Y = mnist.val_Y

if __name__ == "__main__":
    hyperparams["gainfunc"] = eval(hyperparams["gainfunc"])
    root = buildTree(trn_X, trn_Y, list(range(mnist.num_feat)), **hyperparams)
    with open(save_path, "wb") as f:
        pickle.dump(root, f)
    pred = np.array([inferTree(root, val_X[i]) for i in range(val_X.shape[0])])
    print("valid acc", np.average(pred==val_Y))