from env import Env
import gym

class GymEnv(Env):

    def __init__(self, name):
        self.env = gym.make(name)
        self.state_dim = self.env.observation_space.shape
        self.action_dim = self.env.action_space.n

    def reset(self, conf = {}):
        obs = self.env.reset()
        # 旧版gym接口返回单个obs，新版接口返回(obs, info)
        if type(obs) == tuple: obs = obs[0]
        return obs
    
    def step(self, action):
        # 旧版gym接口step返回(obs, reward, done, info)，新版接口返回(obs, reward, done, truncated, info)
        obs, reward, *t = self.env.step(action)
        if len(t) == 2:
            return obs, reward, t[0], t[1]
        else:
            done, truncated, info = t
            return obs, reward, done or truncated, info

import numpy as np
class BreakoutEnv(Env):

    def __init__(self):
        self.env = gym.make('BreakoutDeterministic-v4')
        self.state_dim = (80, 80)
        self.action_dim = 4
    
    def preprocess_obs(self, obs):
        # 为了训练效率，对状态进行简化
        obs = np.mean(obs, axis = 2) # 灰度图
        obs = obs[35:195] # 裁剪中间区域
        obs = obs[::2, ::2] # 下采样
        obs = obs.astype(np.float32) / 256
        return obs
    
    def reset(self, conf = {}):
        obs = self.env.reset()
        # 旧版gym接口返回单个obs，新版接口返回(obs, info)
        if type(obs) == tuple: obs = obs[0]
        return self.preprocess_obs(obs)
    
    def step(self, action):
        obs, reward, *t = self.env.step(action)
        obs = self.preprocess_obs(obs)
        if len(t) == 2:
            return obs, reward, t[0], t[1]
        else:
            done, truncated, info = t
            return obs, reward, done or truncated, info
