# HW1 实验报告

## 井字棋

通过迭代值函数逼近最优策略，使用$\epsilon$-greedy策略平衡exploration和exploitation。

实验参数如下：
```cpp
int num_samples = 10000000; // number of training episodes (1 episode = 1 game)
double alpha = 0.01;    // learning rate
bool alpha_decay = false;   // whether to decay learning rate over time, 
                            // if true, alpha = 1 / # episode
double eps = 0.3;   // probability of choosing random action
```

最简单的策略：由于bot的策略过于简单，可以仅通过让agent和bot采取同样的策略，即每次放在棋盘上的第一个空位达到100%胜率，获胜时盘面如下：
```
X | O | X

O | X | O

X |   |  
```

显然这样并不能说明agent学到了什么。因此我们将`num_samples`和`eps`设置得较大，让agent有更多的探索机会，以说明算法是有效的。

训练过程中胜率的变化如下所示：
```txt
Episode 100000, Win rate: 92.203%
Episode 200000, Win rate: 92.321%
Episode 300000, Win rate: 93.8173%
Episode 400000, Win rate: 94.6102%
Episode 500000, Win rate: 95.1094%
Episode 600000, Win rate: 95.4025%
Episode 700000, Win rate: 95.6279%
Episode 800000, Win rate: 95.8089%
Episode 900000, Win rate: 95.9423%
Episode 1000000, Win rate: 96.0452%
Episode 1100000, Win rate: 96.1319%
Episode 1200000, Win rate: 96.2059%
Episode 1300000, Win rate: 96.2681%
Episode 1400000, Win rate: 96.3277%
Episode 1500000, Win rate: 96.3805%
Episode 1600000, Win rate: 96.4278%
Episode 1700000, Win rate: 96.4659%
Episode 1800000, Win rate: 96.503%
Episode 1900000, Win rate: 96.5341%
Episode 2000000, Win rate: 96.5626%
Episode 2100000, Win rate: 96.5974%
Episode 2200000, Win rate: 96.6412%
Episode 2300000, Win rate: 96.6767%
Episode 2400000, Win rate: 96.7015%
Episode 2500000, Win rate: 96.7236%
Episode 2600000, Win rate: 96.7523%
Episode 2700000, Win rate: 96.7747%
Episode 2800000, Win rate: 96.7935%
Episode 2900000, Win rate: 96.8203%
Episode 3000000, Win rate: 96.8407%
Episode 3100000, Win rate: 96.8539%
Episode 3200000, Win rate: 96.8674%
Episode 3300000, Win rate: 96.8748%
Episode 3400000, Win rate: 96.893%
Episode 3500000, Win rate: 96.9086%
Episode 3600000, Win rate: 96.9176%
Episode 3700000, Win rate: 96.9291%
Episode 3800000, Win rate: 96.9485%
Episode 3900000, Win rate: 96.969%
Episode 4000000, Win rate: 96.9837%
Episode 4100000, Win rate: 96.9936%
Episode 4200000, Win rate: 97.0076%
Episode 4300000, Win rate: 97.0177%
Episode 4400000, Win rate: 97.026%
Episode 4500000, Win rate: 97.0324%
Episode 4600000, Win rate: 97.0403%
Episode 4700000, Win rate: 97.0475%
Episode 4800000, Win rate: 97.0539%
Episode 4900000, Win rate: 97.0594%
Episode 5000000, Win rate: 97.0685%
Episode 5100000, Win rate: 97.0735%
Episode 5200000, Win rate: 97.0799%
Episode 5300000, Win rate: 97.0901%
Episode 5400000, Win rate: 97.0959%
Episode 5500000, Win rate: 97.1015%
Episode 5600000, Win rate: 97.1046%
Episode 5700000, Win rate: 97.1153%
Episode 5800000, Win rate: 97.1221%
Episode 5900000, Win rate: 97.126%
Episode 6000000, Win rate: 97.1329%
Episode 6100000, Win rate: 97.14%
Episode 6200000, Win rate: 97.142%
Episode 6300000, Win rate: 97.1447%
Episode 6400000, Win rate: 97.1488%
Episode 6500000, Win rate: 97.1538%
Episode 6600000, Win rate: 97.1569%
Episode 6700000, Win rate: 97.1588%
Episode 6800000, Win rate: 97.1603%
Episode 6900000, Win rate: 97.1631%
Episode 7000000, Win rate: 97.1633%
Episode 7100000, Win rate: 97.1641%
Episode 7200000, Win rate: 97.1667%
Episode 7300000, Win rate: 97.1719%
Episode 7400000, Win rate: 97.1753%
Episode 7500000, Win rate: 97.1799%
Episode 7600000, Win rate: 97.1819%
Episode 7700000, Win rate: 97.183%
Episode 7800000, Win rate: 97.1869%
Episode 7900000, Win rate: 97.1908%
Episode 8000000, Win rate: 97.1956%
Episode 8100000, Win rate: 97.1987%
Episode 8200000, Win rate: 97.2023%
Episode 8300000, Win rate: 97.2054%
Episode 8400000, Win rate: 97.2095%
Episode 8500000, Win rate: 97.2128%
Episode 8600000, Win rate: 97.2149%
Episode 8700000, Win rate: 97.2175%
Episode 8800000, Win rate: 97.2176%
Episode 8900000, Win rate: 97.2184%
Episode 9000000, Win rate: 97.2211%
Episode 9100000, Win rate: 97.22%
Episode 9200000, Win rate: 97.2212%
Episode 9300000, Win rate: 97.2212%
Episode 9400000, Win rate: 97.2232%
Episode 9500000, Win rate: 97.2247%
Episode 9600000, Win rate: 97.2261%
Episode 9700000, Win rate: 97.229%
Episode 9800000, Win rate: 97.2297%
Episode 9900000, Win rate: 97.2321%
Episode 10000000, Win rate: 97.2312%
```

训练结束后进行一局测试，agent获胜，对局过程如下：
```
Game reset.
Board: 
	_	_	_
	_	_	_
	_	_	_
Next turn: X

Action (0,2) taken.
Board: 
	_	_	X
	_	_	_
	_	_	_
Next turn: O

Winner not found.

Action (0,0) taken.
Board: 
	O	_	X
	_	_	_
	_	_	_
Next turn: X

Winner not found.

Action (2,2) taken.
Board: 
	O	_	X
	_	_	_
	_	_	X
Next turn: O

Winner not found.

Action (0,1) taken.
Board: 
	O	O	X
	_	_	_
	_	_	X
Next turn: X

Winner not found.

Action (2,0) taken.
Board: 
	O	O	X
	_	_	_
	X	_	X
Next turn: O

Winner not found.

Action (1,0) taken.
Board: 
	O	O	X
	O	_	_
	X	_	X
Next turn: X

Winner not found.

Action (1,2) taken.
Board: 
	O	O	X
	O	_	X
	X	_	X
Next turn: O

Winner: X

Winner: X

```

## 多臂老虎机
### $\epsilon$-greedy

<img src="./多臂老虎机/bandit_eps.svg" alt="$\epsilon$-greedy" 
style="width:500px" />

从上图可以看出，过小或者过大的$\epsilon$都会导致平均奖励的下降。

如果$\epsilon$（e.g., 0）过小，agent不能充分进行探索，从而使对动作价值的估计集中在某个固定的动作，因此难以找到最优解。

过大的$\epsilon$（e.g., 0.1, 0.2）会使agent频繁尝试其他动作。尽管随着训练进行对各个动作价值的估计都趋于准确，但由于频繁尝试其他动作，导致无法充分利用当前的最优动作，从而使平均奖励降低。

### UCB

<img src="./多臂老虎机/bandit_ucb.svg" alt="UCB" 
style="width:500px" />

探索（$c\neq 0$）对算法的性能有显著提升。且随着$c$的增大，模型收敛的速度变慢。

### Gradient Bandit Algorithm

<img src="./多臂老虎机/bandit_grad.svg" alt="Gradient Bandit Algorithm" 
style="width:500px" />

在更新动作优先级的过程中加入baseline（当前时刻的平均奖励）理论上可以使算法更稳定（需要多次实验验证，此处未展示），这里从图中可看出有baseline的算法收敛速度略快于没有baseline的算法。

### 优化初值

<img src="./多臂老虎机/bandit_init.svg" alt="优化初值" 
style="width:500px" />

乐观的动作价值初始估计鼓励算法在早期进行大量探索，从而可以更好地找到最优解。相反，悲观的估计使算法倾向于保守，因为早期的探索会导致平均奖励下降。