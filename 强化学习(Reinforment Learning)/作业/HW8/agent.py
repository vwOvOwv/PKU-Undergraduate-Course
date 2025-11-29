from abc import ABC, abstractmethod

# RL算法基类
class Agent(ABC):

    # 训练时采样动作
    @abstractmethod
    def predict(self, obs):
        pass

    # 测试时选最优动作
    @abstractmethod
    def exploit(self, obs):
        pass

    # 从一批样本中训练
    @abstractmethod
    def learn(self, samples):
        pass

    # 样本预处理
    @abstractmethod
    def sample_process(self, samples):
        pass