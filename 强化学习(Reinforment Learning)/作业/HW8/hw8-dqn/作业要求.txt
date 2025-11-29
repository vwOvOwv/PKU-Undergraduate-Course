作业说明：

0. 截止日期：
	2025年11月20日

1. 样例代码：
	【requirements.txt】
		运行pip install -r requirements.txt安装样例代码所需的运行库
	【env.py】
		RL环境基类，不建议修改
	【gym_env.py】
		对gym环境进行简单包装，可以自行修改Breakout的状态预处理代码，其他部分不建议修改
	【agent.py】
		RL算法基类，不建议修改
	【dqn_agent.py】
		DQN算法实现，需要将代码中的空缺补充完整，其他部分不建议修改
	【model.py】
		模型基类，不建议修改
	【q_network.py】
		针对CartPole的简易Q网络实现，可以自由调整结构
	【sample.py】
		为了实现算法代码的可复用性，对帧状态和样本数据进行的抽象，不建议修改
	【dqn_train.py】
		训练代码入口，初始代码为CartPole训练，切换Breakout训练需要修改使用的环境；两者均需要对超参数进行调整；可以自行修改训练指标的呈现方式；可以尝试实现多线程采样的逻辑
	
2. 作业要求和过程指导
	（1）先运行`pip install -r requirements.txt`，安装requirements.txt中的python依赖项
	（2）补全DQNAgent类中的DQN算法代码，使得运行dqn_train.py时能够正常训练CartPole
	（3）调整网络结构以及dqn_train.py中的超参数，提升CartPole上的训练表现
	（4）对Q网络、训练入口代码进行修改，使其适配Breakout环境的训练
	（5）通过调整网络结构、超参数以及对算法进行优化，提升Breakout上的训练表现
	（6）在实验报告中列出并总结尝试过的优化技巧以及实验效果，在教学网提交最终版的代码和实验报告