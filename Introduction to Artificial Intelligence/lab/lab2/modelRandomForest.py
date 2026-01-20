from modelTree import *
from numpy.random import rand
from answerRandomForest import buildtrees, infertrees
from answerTree import *
from modelTree import discretize, trn_X, trn_Y, val_X, val_Y
from util import setseed

setseed(0) # 固定随机数种子以提高可复现性

save_path = "model/rforest.npy"

if __name__ == "__main__":
    hyperparams["gainfunc"] = eval(hyperparams["gainfunc"])
    roots = buildtrees(trn_X, trn_Y)
    with open(save_path, "wb") as f:
        pickle.dump(roots, f)
    pred = np.array([infertrees(roots, val_X[i]) for i in range(val_X.shape[0])])
    print("valid acc", np.average(pred==val_Y))