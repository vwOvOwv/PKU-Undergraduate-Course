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

def epsilon_greedy(values, epsilon):
    assert len(values) > 1, "There should be 2 or more values."
    eps = epsilon * len(values) / (len(values) - 1)
    if random() <= eps:
        return randint(0, len(values)-1)
    return int(np.argmax(values))


if __name__ == "__main__":
    n = 5
    bandit = NormalDistBandit(means = np.array(range(-n, n+1)), stds = np.ones(11))
    
    init_estimates = [-8, -4, 0, 4, 8]
    iter = 10000
    eps = 0.
    
    x = np.array(range(iter))
    y = np.zeros((len(init_estimates), iter), dtype=np.float64)

    values = np.zeros((len(init_estimates), n*2+1), dtype=np.float64)
    counts = np.zeros((len(init_estimates), n*2+1), dtype=np.int64)
    for idx, init_value in enumerate(init_estimates):
        values[idx] += init_value
        for i in range(1, iter):
            action = epsilon_greedy(values[idx], eps)
            counts[idx, action] += 1
            value = bandit.pull(action)
            values[idx, action] = (values[idx, action] * (counts[idx, action] - 1) + value) / counts[idx, action]
            y[idx, i] = (y[idx, i-1] * (i-1) + value) / i

    plt.figure()
    plt.plot(x, y.T)
    plt.legend([f'initial value estimation={init_value}' for init_value in init_estimates],
               loc='lower right')
    plt.title(f'Different Initial Value Estimations')
    plt.xlabel('Iterations')
    plt.ylabel('Average reward')
    # plt.show()
    plt.savefig('./bandit_init.svg')