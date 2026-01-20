'''
 回归。计算accuracy。
'''
import numpy as np
from numpy.random import randn
import mnist
from answerLogisticRegression import predict, step
import pickle
from util import setseed

setseed(0) # 固定随机数种子以提高可复现性

save_path = "model/lr.npy"

X=mnist.trn_X
Y=2*(mnist.trn_Y == 0) - 1 
weight=randn(mnist.num_feat)
bias=np.zeros((1))

if __name__ == "__main__":
    # 训练
    best_train_acc = 0
    for i in range(1, 60+1):
        haty, loss, weight, bias = step(X, weight, bias, Y)
        acc = np.average((haty>0).flatten()==(Y>0))
        print(f"epoch {i} loss {loss:.3e} acc {acc:.4f}")
        if acc > best_train_acc:
            best_train_acc = acc
            with open(save_path, "wb") as f:
                pickle.dump((weight, bias), f)

    # 测试
    with open(save_path, "rb") as f:
        weight, bias = pickle.load(f)
    haty = predict(mnist.val_X, weight, bias)
    haty = (haty>0).flatten()
    y = (mnist.val_Y==0)
    print(f"confusion matrix: TP {np.sum((haty>0)*(y>0))} FN {np.sum((y>0)*(haty<=0))} FP {np.sum((y<=0)*(haty>0))} TN {np.sum((y<=0)*(haty<=0))}")
    print(f"valid acc {np.average(haty==y):.4f}")

