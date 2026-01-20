import numpy as np
from random import random, randint
from time import sleep
from matplotlib import pyplot as plt

class NormalDistBandit:
    def __init__(self, means, stds):
        assert len(means) == len(stds), "Means and stds must be the same length."
        self.n = len(means)
        self.means = np.array(means)
        self.stds = np.array(stds)
        assert all(self.stds >= 0), "Stds must be positive."
    
    def pull(self, k):
        assert 0 <= k < self.n, f"Invalid arm {k}."
        return np.random.normal(loc=self.means[k], scale=self.stds[k])

def ucb(values, counts, c, t):
    assert len(values) > 1, "There should be 2 or more values."
    ucb_values = values + c * np.sqrt(np.log(t) / (counts + 1e-5))
    return int(np.argmax(ucb_values))
        
if __name__ == "__main__":
    n = 5
    bandit = NormalDistBandit(means = np.array(range(-n, n+1)), stds = np.ones(11))

    c_list = [0, 1, 2, 3, 4]
    iter = 10000

    x = np.array(range(iter))
    y = np.zeros((len(c_list), iter), dtype=np.float64)
    values = np.zeros((len(c_list), n*2+1), dtype=np.float64)
    counts = np.zeros((len(c_list), n*2+1), dtype=np.int64)

    for idx, c in enumerate(c_list):
        for i in range(1, iter):
            action = ucb(values[idx], counts[idx], c=c, t=i)
            counts[idx, action] += 1
            value = bandit.pull(action)
            values[idx, action] = (values[idx, action] * (counts[idx, action] - 1) + value) / counts[idx, action]
            y[idx, i] = (y[idx, i-1] * (i-1) + value) / i

    plt.figure()
    plt.plot(x, y.T)
    plt.legend([f'c={c}' for c in c_list])
    plt.title('UCB')
    plt.xlabel('Iterations')
    plt.ylabel('Average reward')
    # plt.show()
    plt.savefig('./bandit_ucb.svg')