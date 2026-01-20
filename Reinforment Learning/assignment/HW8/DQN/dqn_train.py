import numpy as np
from collections import deque
from gym_env import BreakoutEnv
from dqn_agent import DQNAgent
from q_network import QNetwork
from sample import FrameNumpy, SampleBatchNumpy
from collections import deque
import random
from tqdm import tqdm
from matplotlib import pyplot as plt

def get_life(env, info=None):
    if info and 'ale.lives' in info:
        return info['ale.lives']
    if info and 'lives' in info:
        return info['lives']
    if hasattr(env, 'unwrapped'):
        return env.unwrapped.ale.lives()
    return 5
    
def test_one_episode(env, agent):
    obs = env.reset()
    obs, _, _, info = env.step(1)
    done = False
    ret = 0
    lives = get_life(env, info)
    need_fire = False
    
    while not done:
        if need_fire:
            action = 1
            need_fire = False
        else:
            action = agent.exploit(obs)
            
        next_obs, reward, done, info = env.step(action)
        ret += reward
        
        new_lives = get_life(env, info)
        if new_lives < lives and new_lives > 0:
            need_fire = True
        lives = new_lives
        obs = next_obs
    return ret

def get_n_step_return(local_buffer, gamma):
    start_obs, start_action = local_buffer[0][0], local_buffer[0][1]
    
    last_next_obs, last_done = local_buffer[-1][3], local_buffer[-1][4]
    
    running_reward = 0
    for i, transition in enumerate(local_buffer):
        r = transition[2]
        running_reward += r * (gamma ** i)
        
    return start_obs, start_action, running_reward, last_next_obs, last_done, len(local_buffer)

def sample_transfer(local_buffer, replay_buffer, gamma):
    n_step_transition = get_n_step_return(local_buffer, gamma)
    sample = FrameNumpy.from_dict({
        'start_obs': np.array(n_step_transition[0]), 
        'start_action': n_step_transition[1], 
        'n_step_reward': n_step_transition[2], 
        'end_obs': np.array(n_step_transition[3]),
        'done': n_step_transition[4],
        'n_step': n_step_transition[5]
    })
    replay_buffer.append(sample)
    local_buffer.popleft()

# 环境
T_stack = 4
train_env = BreakoutEnv(T=T_stack)
test_env = BreakoutEnv(T=T_stack)

in_channels = T_stack
H_in = train_env.state_dim[0]
W_in = train_env.state_dim[1]
action_dim = train_env.action_dim

# 参数
n_step = 4
gamma = 0.99
conf = dict(
    action_dim = action_dim,
    gamma = gamma,
    device = 'cuda',
    target_update_freq = 10000
)

buffer_size = 100000
batch_size = 32
total_steps = 10000000
tqdm_bar = tqdm(range(total_steps))

epsilon_start = 1.0
epsilon_end = 0.1
epsilon_decay_steps = 1000000
start_learning_steps = 50000

learning_rate = 1e-4

# 模型
agent = DQNAgent(conf)
model = QNetwork(in_channels, action_dim, lr=learning_rate)
agent.set_model(model)
local_buffer = deque(maxlen=n_step)
replay_buffer = deque(maxlen=buffer_size)

# 训练过程
train_returns = []
test_returns = []
num_episode = 0

train_ret = 0
obs = train_env.reset()
need_fire = True
life = 5

for step in tqdm_bar:
    if step < epsilon_decay_steps:
        epsilon = epsilon_start - (epsilon_start - epsilon_end) * (step / epsilon_decay_steps)
    else:
        epsilon = epsilon_end

    action = 1 if need_fire else agent.predict(obs, float(epsilon))
    next_obs, reward, done, info = train_env.step(action)

    train_ret += reward
    clipped_reward = np.clip(reward, -1, 1)

    new_life = get_life(train_env, info)
    need_fire = (new_life < life)   # 包括了五条命用完，一局游戏结束的情况
    local_buffer.append((obs.copy(), action, clipped_reward, next_obs.copy(), need_fire))
    # local_buffer.append((obs, action, reward, next_obs, need_fire))

    if done:
        while len(local_buffer) > 0:
            sample_transfer(local_buffer, replay_buffer, gamma)
    else:
        if len(local_buffer) == n_step:
            sample_transfer(local_buffer, replay_buffer, gamma)

    if len(replay_buffer) > start_learning_steps and step % 4 == 0:
        batch = random.sample(replay_buffer, batch_size)
        batch = SampleBatchNumpy.stack(batch)
        agent.sample_process(batch)
        agent.learn(batch)

    if done: # 一局游戏结束
        train_returns.append((step, train_ret))
        if num_episode % 20 == 0:
            test_ret = test_one_episode(test_env, agent)
            test_returns.append((step, test_ret))
        tqdm_bar.set_description(f"Test Ret: {test_ret:.2f}, Ep #{num_episode}, Ep Ret: {train_ret:.2f}, Eps: {epsilon:.2f}")
        num_episode += 1

        train_ret = 0
        obs = train_env.reset()
        need_fire = True
        life = 5
    else:
        life = new_life
        obs = next_obs


def plot_with_shadow(x, y, color, label, window=10):
    x = np.array(x)
    y = np.array(y)
    
    if len(y) < window:
        window = len(y)
    
    y_smooth = []
    y_std = []
    
    for i in range(len(y)):
        start = max(0, i - window // 2)
        end = min(len(y), i + window // 2 + 1)
        segment = y[start:end]
        y_smooth.append(np.mean(segment))
        y_std.append(np.std(segment))
        
    y_smooth = np.array(y_smooth)
    y_std = np.array(y_std)
    
    plt.fill_between(x, y_smooth - y_std, y_smooth + y_std, color=color, alpha=0.2)
    
    plt.plot(x, y_smooth, color=color, label=label, linewidth=2)

    plt.plot(x, y, color=color, alpha=0.1, linewidth=1)

plt.style.use('seaborn-v0_8-whitegrid')
plt.figure(figsize=(12, 6), dpi=300)

train_x = [x[0] for x in train_returns]
train_y = [x[1] for x in train_returns]
test_x = [x[0] for x in test_returns]
test_y = [x[1] for x in test_returns]

if len(train_x) > 0:
    plot_with_shadow(train_x, train_y, color="#074459", label='Train Return', window=50)
if len(test_x) > 0:
    plot_with_shadow(test_x, test_y, color='#F08080', label='Test Return', window=20)

plt.title(f"Breakout", fontsize=14, fontweight='bold')
plt.xlabel("Steps", fontsize=12)
plt.ylabel("Return", fontsize=12)
plt.legend(loc='upper left', frameon=True, fontsize=10)
plt.grid(False)

plt.tight_layout()
plt.savefig(f"breakout-DDQN-nstep{n_step}-lr{learning_rate}-eps{epsilon}.png")
plt.savefig(f"breakout-DDQN-nstep{n_step}-lr{learning_rate}-eps{epsilon}.pdf")
# plt.show()
