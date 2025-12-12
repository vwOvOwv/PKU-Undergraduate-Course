from env import Env
import gym
import numpy as np
if not hasattr(np, 'bool8'):
    np.bool8 = np.bool_
from collections import deque

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


class BreakoutEnv(Env):

    def __init__(self, T=4):    # 支持帧堆叠
        self.env = gym.make('BreakoutDeterministic-v4')
        self.state_dim = (80, 80)
        self.action_dim = 4
        self.T = T
        self.frames = deque(maxlen=T)
    
    def preprocess_obs(self, obs):
        obs = np.mean(obs, axis = 2) # 灰度图
        obs = obs[35:195] # 裁剪中间区域
        obs = obs[::2, ::2] # 下采样
        obs = obs.astype(np.float32) / 256
        return obs
    
    def reset(self, conf = {}):
        obs = self.env.reset()
        # 旧版gym接口返回单个obs，新版接口返回(obs, info)
        if type(obs) == tuple: obs = obs[0]
        preprocessed_obs = self.preprocess_obs(obs)
        self.frames.clear()
        # 初始化时把第一帧复制T次填满队列
        for _ in range(self.T):
            self.frames.append(preprocessed_obs)
        return self.stack_frames()
    
    def step(self, action):
        obs, reward, *t = self.env.step(action)
        preprocessed_obs = self.preprocess_obs(obs)
        # 更新帧队列
        self.frames.append(preprocessed_obs)
        if len(t) == 2:
            return self.stack_frames(), reward, t[0], t[1]
        else:
            done, truncated, info = t
            return self.stack_frames(), reward, done or truncated, info

    def stack_frames(self):
        return np.stack(self.frames, axis=0)