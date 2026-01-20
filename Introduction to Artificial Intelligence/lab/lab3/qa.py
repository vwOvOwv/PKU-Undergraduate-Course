import fruit
import numpy as np
import pickle
from FruitModel import QAModel
from SST_2.dataset import testdataset
import random

def setseed(seed):
    np.random.seed(seed)
    random.seed(seed)

setseed(0) # 固定随机数种子以提高可复现性

if __name__ == "__main__":
    network = QAModel()
    while True:
        query = input('Q: ')
        if query == 'exit':
            break
        print('A: {}'.format(network(query)))
