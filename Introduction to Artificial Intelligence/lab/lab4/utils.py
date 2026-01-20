import numpy as np


class Particle:
    def __init__(self, x=0., y=0., theta=0., weight=0.) -> None:
        self.position = np.array([x, y]) # 粒子位置
        self.theta = theta # 粒子朝向
        self.weight = weight # 权重
    
    def get_weight(self):
        return self.weight


class TreeNode:
    def __init__(self, p_idx, x, y) -> None:
        self.parent_idx = p_idx
        self.pos = np.array([x, y])
