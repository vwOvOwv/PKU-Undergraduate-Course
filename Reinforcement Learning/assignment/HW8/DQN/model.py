from abc import ABC, abstractmethod

# 模型基类
class Model(ABC):

    # 模型推理（前向传播）
    @abstractmethod
    def inference(self, obs):
        pass

    # 模型训练（反向传播）
    @abstractmethod
    def train(self, loss):
        pass