# 环境
from gym_env import GymEnv
env = GymEnv('CartPole-v0')
state_dim = env.state_dim[0]
action_dim = env.action_dim

# 算法
from dqn_agent import DQNAgent
conf = dict(
    action_dim = action_dim,
    epsilon = 0.02,
    gamma = 1,
    device = 'cpu'
)
agent = DQNAgent(conf)

# 模型
from q_network import QNetwork
model = QNetwork(state_dim, action_dim, lr = 1e-3)
agent.set_model(model)


from sample import FrameNumpy, SampleBatchNumpy
from collections import deque
import random
from tqdm import tqdm
from matplotlib import pyplot as plt

# 训练流程
buffer_size = 1000
batch_size = 32
episodes = 1000
train_returns = []
test_returns = []
replay_buffer = deque(maxlen = buffer_size) # 样本池
for episode in tqdm(range(episodes)):
    ret = 0
    obs = env.reset()
    done = False
    while not done:
        action = agent.predict(obs)
        next_obs, reward, done, _ = env.step(action)
        ret += reward
        sample = FrameNumpy.from_dict({
            'obs': obs,
            'next_obs': next_obs,
            'action': action,
            'reward': reward,
            'done': done
        })
        obs = next_obs
        # 每个step产生的样本加入样本池，并直接采样batch进行单次训练
        replay_buffer.append(sample)
        if len(replay_buffer) > batch_size:
            batch = random.sample(replay_buffer, batch_size)
            batch = SampleBatchNumpy.stack(batch)
            agent.sample_process(batch)
            agent.learn(batch)
    train_returns.append((episode, ret))
    if episode % 10 == 0:
        # 每10局测试一局效果
        ret = 0
        obs = env.reset()
        done = False
        while not done:
            action = agent.exploit(obs) # 最优动作
            next_obs, reward, done, _ = env.step(action)
            ret += reward
            obs = next_obs
        test_returns.append((episode, ret))

plt.plot([x[0] for x in train_returns], [x[1] for x in train_returns], label = 'train')
plt.plot([x[0] for x in test_returns], [x[1] for x in test_returns], label = 'test')
plt.legend()
plt.title("CartPole")
plt.show()