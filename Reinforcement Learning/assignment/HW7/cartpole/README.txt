DiscreteCartPole
  在DiscreteCartPole环境上实现n-step TD学习算法，对比不同n（至少3种）对算法效果的影响，撰写实验报告并提交代码。
提示：
  1. 需要安装gym包：pip install gym
  2. python样例程序中已经实现离散化的CartPole环境以及其上的Q学习算法。
  3. 可以尝试调整训练的各个超参数（包括离散化的粒度）来获得更好的训练效果。
  4. 注意样例代码使用的旧版gym接口调用形式为：
	state = env.reset()
	next_state, reward, terminated, info = env.step(action)
     新版gym接口调用形式为：
	state, info = env.reset()
	next_state, reward, terminated, truncated, info = env.step(action)
     如解释器在对应行报错，请按照本地使用的gym版本调整上述代码。

