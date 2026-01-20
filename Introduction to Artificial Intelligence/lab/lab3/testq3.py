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

data = [
    ('How many tonnes of apples will be produced in 2021?', 'Worldwide production of apples in 2021 was 93 million tonnes, with China accounting for nearly half of the total.'),
    ('What is the key components of games?', 'Key components of games are goals, rules, challenge, and interaction.'),
    ('Which year was the first PlayStation console released?', 'The brand is produced by Sony Interactive Entertainment, a division of Sony; the first PlayStation console was released in Japan in December 1994, and worldwide the following year.'),    
]

if __name__ == "__main__":
    network = QAModel()
    acc = 0
    cnt = 0
    for text, ans in data:
        output = network(text)
        acc += (output == ans)
        cnt += 1
    print("valid acc", 1.0*acc/cnt)
