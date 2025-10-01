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

def gradient_bandit(priorities):
    assert len(priorities) > 1, "There should be 2 or more values."
    cur_priorities = priorities.copy()
    cur_priorities -= cur_priorities.max()    # for numerical stability
    exp = np.exp(cur_priorities)
    prob = exp / np.sum(exp)
    return np.random.choice(len(prob), p=prob), prob    # sample from softmax distribution
    
if __name__ == "__main__":
    n = 5
    bandit = NormalDistBandit(means = np.array(range(-n, n+1)), stds = np.ones(11))
    
    alpha = 0.1
    iter = 10000
    
    x = np.array(range(iter))
    y = np.zeros((2, iter), dtype=np.float64)

    priorities = np.zeros((2, n*2+1), dtype=np.float64)
    values = np.zeros((2, n*2+1), dtype=np.float64)
    counts = np.zeros((2, n*2+1), dtype=np.int64)
    for idx in range(2):
        for i in range(1, iter):
            action, prob = gradient_bandit(priorities[idx])
            counts[idx, action] += 1
            value = bandit.pull(action)
            values[idx, action] = (values[idx, action] * (counts[idx, action] - 1) + value) / counts[idx, action]
            y[idx, i] = (y[idx, i-1] * (i-1) + value) / i
            # update priorities
            if idx == 1:
                priorities[idx, action] += alpha * (value - y[idx, i]) * (1 - prob[action])
                prob[action] = 0.
                priorities[idx] -= alpha * (value - y[idx, i]) * prob
            else:
                priorities[idx, action] += alpha * (value - 0) * (1 - prob[action])
                prob[action] = 0.
                priorities[idx] -= alpha * (value - 0) * prob

    plt.figure()
    plt.plot(x, y.T)
    plt.legend([f'baseline={baseline}' for baseline in [False, True]])
    plt.title(f'Gradient Bandit, alpha={alpha}')
    plt.xlabel('Iterations')
    plt.ylabel('Average reward')
    # plt.show()
    plt.savefig('./bandit_grad.svg')