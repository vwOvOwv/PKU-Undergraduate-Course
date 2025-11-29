from abc import ABC, abstractmethod

# RL环境基类
class Env(ABC):

    # 开始新的episode
    @abstractmethod
    def reset(self, conf):
        '''
        obs = env.reset(conf)
        '''
        pass

    # 单步状态转移
    @abstractmethod
    def step(self, action):
        '''
        obs, reward, done, info = env.step(action)
        '''
        pass