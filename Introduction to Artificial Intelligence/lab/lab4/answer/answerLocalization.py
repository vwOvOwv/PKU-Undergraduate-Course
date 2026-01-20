from typing import List
import numpy as np
from utils import Particle

### 可以在这里写下一些你需要的变量和函数 ###
COLLISION_DISTANCE = 1
MAX_ERROR = 50000

### 可以在这里写下一些你需要的变量和函数 ###
k=1 #weight函数参数
sigma1=np.sqrt(0.008) #位置标准差
sigma2=np.sqrt(0.0015) #角度标准差
portion=0.6 #重采样时随机取样的比例
n=6 #计算平均结果时取的样本数量

def generate_uniform_particles(walls, N):
    """
    输入：
    walls: 维度为(xxx, 2)的np.array, 地图的墙壁信息，具体设定请看README关于地图的部分
    N: int, 采样点数量
    输出：
    particles: List[Particle], 返回在空地上均匀采样出的N个采样点的列表，每个点的权重都是1/N
    """
    all_particles: List[Particle] = []
    for _ in range(N):
        all_particles.append(Particle(1.0, 1.0, 1.0, 0.0))
    ### 你的代码 ###
    #设置地图范围
    x_min=np.min(walls[:,0])
    x_max=np.max(walls[:,0])
    y_min=np.min(walls[:,1])
    y_max=np.max(walls[:,1])
    #在空地上随机均匀生成样本点
    for i in range(N):
        while True:
            x=np.random.uniform(x_min,x_max)
            y=np.random.uniform(y_min,y_max)
            flag=1
            #判断是否在空地上
            for wall in walls:
                if x<x_min or x>x_max or y<y_min or y>y_max:
                    flag=0
                    break
                if x<wall[0]+0.5 and x>wall[0]-0.5 and y<wall[1]+0.5 and y>wall[1]-0.5:
                    flag=0
                    break
            if flag:
                all_particles[i].position=np.array([x,y])
                all_particles[i].weight=1./N
                all_particles[i].theta=np.random.uniform(-np.pi,np.pi)
                break
    ### 你的代码 ###
    return all_particles


def calculate_particle_weight(estimated, gt):
    """
    输入：
    estimated: np.array, 该采样点的距离传感器数据
    gt: np.array, Pacman实际位置的距离传感器数据
    输出：
    weight, float, 该采样点的权重
    """
    weight = 1.0
    ### 你的代码 ###
    error=gt-estimated
    weight=np.exp(-k*np.linalg.norm(error))
    # print(weight)
    ### 你的代码 ###
    return weight


def resample_particles(walls, particles: List[Particle]):
    """
    输入：
    walls: 维度为(xxx, 2)的np.array, 地图的墙壁信息，具体设定请看README关于地图的部分
    particles: List[Particle], 上一次采样得到的粒子，注意是按权重从大到小排列的
    输出：
    particles: List[Particle], 返回重采样后的N个采样点的列表
    """
    resampled_particles: List[Particle] = []
    for _ in range(len(particles)):
        resampled_particles.append(Particle(1.0, 1.0, 1.0, 0.0))
    ### 你的代码 ###
    N=len(particles)
    j=0
    for i in range(int(portion*N)):
        resampledParticleNum=int(N*particles[i].weight)
        cnt=0
        while cnt<resampledParticleNum:
            x=particles[i].position[0]+np.random.normal(0,sigma1)
            y=particles[i].position[1]+np.random.normal(0,sigma1)
            resampled_particles[j].position[0]=x
            resampled_particles[j].position[1]=y
            resampled_particles[j].theta=particles[i].theta+np.random.normal(0,sigma2)
            resampled_particles[j].theta=(resampled_particles[j].theta+np.pi)%(2*np.pi)-np.pi
            cnt+=1
            j+=1
            if j>=int(portion*N):
                break
        if j>=int(portion*N):
            break
    tmpN=N-j
    tmp=generate_uniform_particles(walls,tmpN)
    for i in range(tmpN):
        resampled_particles[j]=tmp[i]
        j+=1
    ### 你的代码 ###
    return resampled_particles

def apply_state_transition(p: Particle, traveled_distance, dtheta):
    """
    输入：
    p: 采样的粒子
    traveled_distance, dtheta: ground truth的Pacman这一步相对于上一步运动方向改变了dtheta，并移动了traveled_distance的距离
    particle: 按照相同方式进行移动后的粒子
    """
    ### 你的代码 ###
    p.theta+=dtheta
    p.theta=(p.theta+np.pi) % (2*np.pi)-np.pi
    p.position[0]+=traveled_distance*np.cos(p.theta)
    p.position[1]+=traveled_distance*np.sin(p.theta)
    ### 你的代码 ###
    return p

def get_estimate_result(particles: List[Particle]):
    """
    输入：
    particles: List[Particle], 全部采样粒子
    输出：
    final_result: Particle, 最终的猜测结果
    """
    final_result = Particle()
    ### 你的代码 ###
    position=np.array([0.,0.])
    weight=0.
    theta=0.
    particles.sort(key=lambda x: x.weight, reverse=True)
    for i in range(n):
        position+=particles[i].position
        weight+=particles[i].weight
        theta+=particles[i].theta
    final_result.position=position/n
    final_result.weight=weight/n
    final_result.theta=theta/n
    ### 你的代码 ###
    return final_result