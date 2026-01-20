import numpy as np
from physicsWrapper import PhysicsInfo


def calc_pd_force(target_pos, current_pos, current_vel, kp, kd):
    """
    输入：
    target_pos: 维度为(2,)的np.array，pacman目标位置
    current_pos: 维度为(2,)的np.array，pacman当前位置
    current_vel: 维度为(2,)的np.array，pacman当前速度
    kp, kd: PD 控制器的比例和微分增益参数
    
    输出：
    force: 维度为(2,)的np.array，施加到pacman上的PD控制力
    """
    force = np.zeros(2,)
    ### 你的代码 ###
    force=kp*(target_pos-current_pos)-kd*current_vel
    ### 你的代码 ###
    return force