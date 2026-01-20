from agent import Agent
import numpy as np
import torch
import torch.nn.functional as F
import copy
from sample import Frame, SampleBatchNumpy

# DQN算法
class DQNAgent(Agent):

    def __init__(self, conf):
        # ε-贪心算法所需的参数
        self.action_dim = conf['action_dim']
        # 累计回报的衰减系数
        self.gamma = conf.get('gamma', 0.99)
        # 设备信息，如'cpu', 'cuda:0'等
        self.device = conf.get('device', 'cpu')
        # 目标网络更新频率
        self.target_update_freq = conf.get('target_update_freq', 2000)
        self.learn_step_counter = 0
    
    # 设置推理和训练使用的模型
    def set_model(self, model):
        self.model = model
        self.model.to(self.device)
        self.target_model = copy.deepcopy(model)
        self.target_model.to(self.device)
        self.target_model.eval()

    # 输入状态，采样探索性动作
    def predict(self, obs: 'Frame | np.ndarray | dict', epsilon: float):
        obs = Frame.convert(obs)
        if np.random.random() < epsilon:
            # 以 ε 的概率随机选择动作
            action = np.random.randint(0, self.action_dim)
        else:
            # 以 1-ε 的概率选择Q值最大的动作
            obs = obs.to_torch(device = self.device)
            with torch.no_grad():
                q_value = self.model.inference(obs)
            action = torch.argmax(q_value, dim=1).item()
        return action
    
    # 输入状态，计算最优动作
    def exploit(self, obs: 'Frame | np.ndarray | dict'):
        # 直接选择Q值最大的动作
        obs = Frame.convert(obs)
        obs = obs.to_torch(device = self.device)
        with torch.no_grad():
            q_value = self.model.inference(obs)
        action = torch.argmax(q_value, dim=1).item()
        return action
    
    # 输入样本Batch，训练模型
    def learn(self, samples: SampleBatchNumpy):
        '''
        samples需要包含字段: obs, next_obs, action, reward, done, n_step
        '''
        samples = samples.to_torch(device = self.device)

        # 计算Q(s, a)
        q_values = self.model.inference(samples.start_obs)
        q_value = q_values.gather(1, samples.start_action)

        if np.random.random() < 0.001:
            print(f"Avg Q: {q_value.mean().item():.2f}, Max Q: {q_value.max().item():.2f}")

        # 计算Q(s', a)
        with torch.no_grad():
            end_q_values = self.model.inference(samples.end_obs)
            next_actions = torch.argmax(end_q_values, dim=1, keepdim=True)

            target_q_values = self.target_model.inference(samples.end_obs)
            end_q_value = target_q_values.gather(1, next_actions)

            gamma_n = self.gamma ** samples.n_step
            q_target = samples.n_step_reward + gamma_n * (1 - samples.done) * end_q_value

        loss = F.smooth_l1_loss(q_value, q_target)
        self.model.update(loss)

        self.learn_step_counter += 1
        if self.learn_step_counter % self.target_update_freq == 0:
            self.target_model.load_state_dict(self.model.state_dict())
    
    def sample_process(self, samples: SampleBatchNumpy):
        '''
        samples需要包含字段: start_obs, start_action, n_step_reward, end_obs, done, n_step
        '''
        samples.start_action = samples.start_action.reshape((-1, 1)).astype(np.int64)
        samples.n_step_reward = samples.n_step_reward.reshape((-1, 1)).astype(np.float32)
        samples.done = samples.done.reshape((-1, 1)).astype(np.float32)
        samples.n_step = samples.n_step.reshape((-1, 1)).astype(np.int64)