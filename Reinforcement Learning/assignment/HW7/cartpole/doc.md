# HW7 实验报告

在离散化的CartPole环境中实现了n-step TD，并比较了不同n值下的训练效果

## n-step TD的实现

提供的代码实现了带有经验回放的Q-learning，为了尽可能使用原有的代码结构和函数接口，实现了带有经验回放
的n-step TD算法。主要修改包括：

### 修改`train`
- 当临时的n-step缓冲区存满$n$步时，计算这$n$步的累积折扣奖励$G^{(n)}$
- 将一个n-step的转换($S_t, A_t, G^{(n)}, S_{t+n}$, done) 放入buffer
- 在episode结束时，将缓冲区中剩余的（不足$n$步的）轨迹，计算其到episode结束时的真实reward，放入buffer

### 修改`update_q`
- 从buffer中采样n-step的转换轨迹
- 用n-step的目标值$G^{(n)} + \gamma^n \max_{a'} Q(S_{t+n}, a')$更新q-table

### 其他修改
- 更新了evaluation，用100个episode的平均reward作为评估指标
- 对新版的gym环境做了适配

## 实验结果

下面展示了不同方法的测试结果（100个episode的平均reward）

|Method|Avg. Reward|
|---|---|
|Q-Learning|149.26|
|n-step TD (n=1)|144.67|
|n-step TD (n=2)|175.67|
|n-step TD (n=4)|197.62|
|n-step TD (n=8)|212.51|
|n-step TD (n=16)|212.04|
|n-step TD (n=32)|153.47|

- 1-step TD的效果与Q-learning相近，二者实际上是等价的

- 随着n的增加，bias减小，variance增大，因此需要选择合适的n以权衡bias和variance

- n=8时效果较佳