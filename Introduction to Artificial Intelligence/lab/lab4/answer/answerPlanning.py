import numpy as np
from typing import List
from utils import TreeNode
from simuScene import PlanningMap


### 定义一些你需要的变量和函数 ###
STEP_DISTANCE = 0.5
TARGET_THREHOLD = 0.25

### 定义一些你需要的变量和函数 ###


class RRT:
    def __init__(self, walls) -> None:
        """
        输入包括地图信息，你需要按顺序吃掉的一列事物位置 
        注意：只有按顺序吃掉上一个食物之后才能吃下一个食物，在吃掉上一个食物之前Pacman经过之后的食物也不会被吃掉
        """
        self.map = PlanningMap(walls)
        self.walls = walls
        
        # 其他需要的变量
        ### 你的代码 ###      
        self.x_min=np.min(walls[:,0])
        self.x_max=np.max(walls[:,0])
        self.y_min=np.min(walls[:,1])
        self.y_max=np.max(walls[:,1])
        
        self.get_target_times=None
        self.curNode=None
        self.MAX_TRY_TIMES=2
        ### 你的代码 ###
        
        # 如有必要，此行可删除
        self.path = None
        
        
    def find_path(self, current_position, next_food):
        """
        在程序初始化时，以及每当 pacman 吃到一个食物时，主程序会调用此函数
        current_position: pacman 当前的仿真位置
        next_food: 下一个食物的位置
        
        本函数的默认实现是调用 build_tree，并记录生成的 path 信息。你可以在此函数增加其他需要的功能
        """
        
        ### 你的代码 ###      
        # print('find_path called')#
        self.path = self.build_tree(current_position, next_food)
        # print(len(self.path))#
        self.get_target_times=np.zeros(len(self.path))
        self.curNode=0
        ### 你的代码 ###
        
        
    def get_target(self, current_position, current_velocity):
        """
        主程序将在每个仿真步内调用此函数，并用返回的位置计算 PD 控制力来驱动 pacman 移动
        current_position: pacman 当前的仿真位置
        current_velocity: pacman 当前的仿真速度
        一种可能的实现策略是，仅作为参考：
        （1）记录该函数的调用次数
        （2）假设当前 path 中每个节点需要作为目标 n 次
        （3）记录目前已经执行到当前 path 的第几个节点，以及它的执行次数，如果超过 n，则将目标改为下一节点
        
        你也可以考虑根据当前位置与 path 中节点位置的距离来决定如何选择 target
        
        同时需要注意，仿真中的 pacman 并不能准确到达 path 中的节点。你可能需要考虑在什么情况下重新规划 path
        """
        target_pose = np.zeros_like(current_position)
        ### 你的代码 ###
        # print('get_target called')#
        # print(self.curNode)#
        self.get_target_times[self.curNode]+=1
        target_pose=2*self.path[self.curNode]-current_position-0.1*current_velocity
        # target_pose=self.path[self.curNode]
        # print(self.get_target_times)#
        if self.get_target_times[self.curNode]>=self.MAX_TRY_TIMES or np.linalg.norm(self.path[self.curNode]-current_position)<=0.5:
            if self.curNode<len(self.path)-1:
                self.curNode+=1
            else:
                return target_pose
        ### 你的代码 ###
        return target_pose
        
    ### 以下是RRT中一些可能用到的函数框架，全部可以修改，当然你也可以自己实现 ###
    def build_tree(self, start, goal):
        """
        实现你的快速探索搜索树，输入为当前目标食物的编号，规划从 start 位置食物到 goal 位置的路径
        返回一个包含坐标的列表，为这条路径上的pd targets
        你可以调用find_nearest_point和connect_a_to_b两个函数
        另外self.map的checkoccupy和checkline也可能会需要，可以参考simuScene.py中的PlanningMap类查看它们的用法
        """
        path = []
        graph:List[TreeNode] = []
        graph.append(TreeNode(-1, start[0], start[1]))
        dis=None
        ### 你的代码 ###
        while(1):
            randnum=np.random.uniform()
            if randnum>0.5:
                rand=TreeNode(0,np.random.uniform(self.x_min+0.75,self.x_max-0.75),np.random.uniform(self.y_min+0.75,self.y_max-0.75))
            else:
                rand=TreeNode(0,goal[0],goal[1])
            # rand=TreeNode(0,goal[0],goal[1])
            # print('rand',rand.pos)#
            nearest_idx,nearest_dis=self.find_nearest_point(rand.pos,graph)
            # print('nearest_idx',nearest_idx)#
            near=graph[nearest_idx]
            noObstacle,tmpNew=self.connect_a_to_b(near.pos,rand.pos)
            if noObstacle and self.map.checkoccupy(tmpNew)==False:
                new=TreeNode(nearest_idx,tmpNew[0],tmpNew[1])
                graph.append(new)
                dis=np.linalg.norm(new.pos-goal)
                # print('dis',dis)#
                if dis<=TARGET_THREHOLD:
                    # print('finished')#
                    cur=graph[-1]
                    while cur.parent_idx!=-1:
                        path.append(cur.pos)
                        cur=graph[cur.parent_idx]
                    path.append(cur.pos)
                    path.reverse()
                    break
        ### 你的代码 ###
        # print('return path')
        return path

    @staticmethod
    def find_nearest_point(point, graph):
        """
        找到图中离目标位置最近的节点，返回该节点的编号和到目标位置距离、
        输入：
        point：维度为(2,)的np.array, 目标位置
        graph: List[TreeNode]节点列表
        输出：
        nearest_idx, nearest_distance 离目标位置距离最近节点的编号和距离
        """
        nearest_idx = -1
        nearest_distance = 10000000.
        ### 你的代码 ###
        for i in range(len(graph)):
            dis=np.linalg.norm(graph[i].pos-point)
            # print('dis',dis)
            if dis<nearest_distance:
                nearest_distance=dis
                nearest_idx=i
            if nearest_distance<TARGET_THREHOLD:
                return nearest_idx, nearest_distance
        ### 你的代码 ###
        return nearest_idx, nearest_distance
    
    def connect_a_to_b(self, point_a, point_b):
        """
        以A点为起点，沿着A到B的方向前进STEP_DISTANCE的距离，并用self.map.checkline函数检查这段路程是否可以通过
        输入：
        point_a, point_b: 维度为(2,)的np.array，A点和B点位置，注意是从A向B方向前进
        输出：
        is_empty: bool，True表示从A出发前进STEP_DISTANCE这段距离上没有障碍物
        newpoint: 从A点出发向B点方向前进STEP_DISTANCE距离后的新位置，如果is_empty为真，之后的代码需要把这个新位置添加到图中
        """
        is_empty = False
        newpoint = np.zeros(2)
        ### 你的代码 ###
        ab=point_b-point_a
        len_ab=np.linalg.norm(ab)
        ac=STEP_DISTANCE/len_ab*ab
        newpoint=ac+point_a
        # print('a',point_a)#
        # print('b',point_b)#
        # print('new',newpoint)#
        hit,hit_position=self.map.checkline(point_a.tolist(),newpoint.tolist())
        if hit==False:
            is_empty=True
        ### 你的代码 ###
        return is_empty, newpoint
