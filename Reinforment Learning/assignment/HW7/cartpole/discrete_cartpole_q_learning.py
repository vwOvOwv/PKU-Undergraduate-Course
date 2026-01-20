import gym
import numpy as np
import tqdm
import random
import math
from typing import Tuple, Dict, Any
import os

class DiscreteCartPoleEnv(gym.Env):
    def __init__(self, intervals=16, render_mode=None):
        self._env = gym.make('CartPole-v1', render_mode=render_mode)
        self.action_space = self._env.action_space
        self.intervals = intervals
        self.observation_space = gym.spaces.MultiDiscrete([intervals]*4)
        self._to_discrete = lambda x, a, b: int(min(max(0, (x-a)*self.intervals/(b-a)), self.intervals))
    
    def reset(self):
        state, info = self._env.reset()
        return self._discretize(state)

    def _discretize(self, state:np.array)->Tuple:
        cart_pos, cart_v, pole_angle, pole_v = state
        cart_pos = self._to_discrete(cart_pos, -2.4, 2.4)
        cart_v = self._to_discrete(cart_v, -3.0, 3.0)
        pole_angle = self._to_discrete(pole_angle, -0.5, 0.5)
        pole_v = self._to_discrete(pole_v, -2.0, 2.0)
        return (cart_pos, cart_v, pole_angle, pole_v)
    
    def step(self, action:int)->Tuple[Tuple, float, bool, Any]:
        state, reward, terminated, truncated, info = self._env.step(action)
        done = terminated or truncated
        state = self._discretize(state)
        return state, reward, done, info

class QLearner:
    def __init__(self, config:Dict):
        for k, v in config.items():
            setattr(self, k, v)
        self.epsilon = self.epsilon_lower
        self.lr = self.lr_upper
        self.buffer = list()
        self.buffer_pointer = 0

    
    def add_to_buffer(self, data):
        if len(self.buffer) < self.buffer_size:
            self.buffer.append(data)
        else:
            self.buffer[self.buffer_pointer] = data
        self.buffer_pointer += 1
        self.buffer_pointer %= self.buffer_size
    
    def sample_batch(self):
        return random.sample(self.buffer, self.batch_size)
    
    def greedy(self, state:Tuple)->int:
        return self.q[state].argmax()

    def epsilon_greedy(self, state:Tuple)->int:
        if random.random() < self.epsilon:
            return self.env.action_space.sample()
        return self.greedy(state)
    
    def epsilon_decay(self, total_step):
        self.epsilon = self.epsilon_lower + (self.epsilon_upper - self.epsilon_lower) * math.exp(-total_step / self.epsilon_decay_freq)
    
    def lr_decay(self, total_step):
        self.lr = self.lr_lower + (self.lr_upper - self.lr_lower) * math.exp(-total_step / self.lr_decay_freq)
    
    def update_q(self, total_step):
        if total_step % self.update_freq != 0 or len(self.buffer) < self.batch_size:
            return
        batch = self.sample_batch()
        for state, action, reward, new_state in batch:
            self.q[state][action] += self.lr * (self.gamma * self.q[new_state].max() + reward - self.q[state][action])
    
    def train(self):
        total_step = 0
        for i in tqdm.trange(self.start_iter, self.iter):
            state = self.env.reset()
            done = False
            while not done:
                total_step += 1
                action = self.epsilon_greedy(state)
                self.epsilon_decay(total_step)
                new_state, reward, done, _ = self.env.step(action)
                if done:
                    reward = self.end_reward
                self.add_to_buffer((state, action, reward, new_state))
                self.update_q(total_step)
                self.lr_decay(total_step)
                state = new_state
            self.save_model(i)
    
    def save_model(self, i):
        if i % self.save_freq == 0:
            np.save(os.path.join(self.save_path, f'{i}.npy'), self.q)
        
            
             
if __name__ == '__main__':
    env_name = 'DiscreteCartPole'
    save_path = 'q_tables'
    intervals = 8
    
    train_env = DiscreteCartPoleEnv(intervals)
    
    q_table = np.zeros(shape=(intervals+1,)*train_env.observation_space.shape[0]+(train_env.action_space.n,))
    
    latest_checkpoint = 0
    
    if save_path not in os.listdir():
        os.mkdir(save_path)
    elif len(os.listdir(save_path)) != 0:
        latest_checkpoint = max([int(file_name.split('.')[0]) for file_name in os.listdir(save_path)])
        print(f'{latest_checkpoint}.npy loaded')
        q_table = np.load(os.path.join(save_path, f'{latest_checkpoint}.npy'))
            
    trainer = QLearner({
        'env':train_env,
        'env_name':env_name,
        'end_reward':-1,
        'q':q_table,
        'start_iter':latest_checkpoint,
        'iter':latest_checkpoint+1000,
        'batch_size':128,
        'buffer_size':10000,
        'gamma':0.9,
        'update_freq':1,
        'epsilon_lower':0.05,
        'epsilon_upper':0.8,
        'epsilon_decay_freq':200,
        'lr_lower':0.05,
        'lr_upper':0.5,
        'lr_decay_freq':200,
        'save_path':save_path,
        'save_freq':50
    })
    trainer.train()
    train_env.close()

    # eval_env = DiscreteCartPoleEnv(intervals, render_mode='human')
    eval_env = DiscreteCartPoleEnv(intervals)
    total_eval_reward = 0
    num_eval_episodes = 100
    for _ in range(num_eval_episodes):
        state = eval_env.reset()
        done = False
        episode_reward = 0
        while not done:
            action = trainer.greedy(state)
            state, reward, done, _ = eval_env.step(action)
            episode_reward += reward
        total_eval_reward += episode_reward
    print(f'Average evaluation reward: {total_eval_reward / num_eval_episodes}')
    eval_env.close()