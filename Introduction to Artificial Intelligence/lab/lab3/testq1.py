import fruit
import numpy as np
import pickle
from FruitModel import NaiveBayesModel
from SST_2.dataset import testdataset
import random

def setseed(seed):
    np.random.seed(seed)
    random.seed(seed)

setseed(0) # 固定随机数种子以提高可复现性

if __name__ == "__main__":
    network = NaiveBayesModel()
    acc = 0
    cnt = 0
    dataloader = testdataset()
    for text, label in dataloader:
        pred = network(text)
        acc += (label == pred)
        cnt += 1
    print("valid acc", 1.0*acc/cnt)
