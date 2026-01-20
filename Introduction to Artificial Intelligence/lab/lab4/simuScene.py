import numpy as np
from Box2D import *
from loadMap import Layout


class RayCastClosestCallback(b2RayCastCallback):
    """This class is from example code of box2d-py"""
    """This callback finds the closest hit"""

    def __repr__(self):
        return 'Closest hit'

    def __init__(self, **kwargs):
        b2RayCastCallback.__init__(self, **kwargs)
        self.fixture = None
        self.hit = False

    def ReportFixture(self, fixture, point, normal, fraction):
        '''
        Called for each fixture found in the query. You control how the ray
        proceeds by returning a float that indicates the fractional length of
        the ray. By returning 0, you set the ray length to zero. By returning
        the current fraction, you proceed to find the closest point. By
        returning 1, you continue with the original ray clipping. By returning
        -1, you will filter out the current fixture (the ray will not hit it).
        '''
        self.hit = True
        # self.fixture = fixture
        self.point = b2Vec2(point)
        # self.normal = b2Vec2(normal)
        # NOTE: You will get this error:
        #   "TypeError: Swig director type mismatch in output value of
        #    type 'float32'"
        # without returning a value
        return fraction


class Scene2D:
    def __init__(self, layout: Layout, num_sensors=16) -> None:
        self.world = b2World(gravity=(0, 0))
        self.groundBodies = []
        self.walls = None
        self.foods = None
        self.gt_food = None
        self.pacman: Box2D.b2Body = None
        self.layout2scene(layout)
        self.pre_simulation_func = None
        self.post_simulation_func = None
        self.run = True
        self.checkpoint = None
        self.arrive = False
        self.use_time = 0
        self.timeStep = 1.0 / 60
        self.vel_iters, self.pos_iters = 6, 2
        self.raycast_callback = RayCastClosestCallback()
        
        self.num_sensors = num_sensors
        ### 环绕一圈的16线激光雷达 ###
        self.lidar_sensor_layout = np.arange(num_sensors) / num_sensors * np.pi * 2 - np.pi
    
    def layout2scene(self, layout: Layout):
        self.walls = np.array(layout.walls)
        for wall in layout.walls:
            groundBody = self.world.CreateStaticBody(position=wall, shapes=b2PolygonShape(box=(0.5 + 1e-6, 0.5 + 1e-6)))
            self.groundBodies.append(groundBody)
        self.foods = layout.foods
        if layout.pacman_pos is not None:
            self.pacman = self.world.CreateDynamicBody(position=layout.pacman_pos)
            self.pacman.CreateCircleFixture(radius=0.25, friction=0.3, density=1.0)
    
    def apply_force(self, force: np.array):
        force_length = np.linalg.norm(force) + 1e-6
        force_direction = force / force_length
        force_length = np.clip(force_length, a_min=1e-6, a_max=20)
        force = force_direction * force_length
        self.pacman.ApplyForceToCenter((force[0], force[1]), wake=True)
        
    def step_once(self):
        if self.pre_simulation_func:
            self.pre_simulation_func()
        self.world.Step(self.timeStep, self.vel_iters, self.pos_iters)
        self.world.ClearForces()
        if self.post_simulation_func:
            self.post_simulation_func()
        # self.eat_food()
    
    def check_eat(self, food_idx):
        # 检查编号为food_idx的食物是否被吃掉
        return self.pacman.fixtures[0].shape.TestPoint(self.pacman.transform, self.foods[food_idx].tolist())

    #@profile
    def lidar_sensor(self, start, moving_direction, length=50.):
        angle = moving_direction
        angles = self.lidar_sensor_layout + angle
        dist = np.stack([length * np.cos(angles), length*np.sin(angles)], axis=-1)
        ends = np.array(start).reshape(1,2) + dist
        
        lidar_results = np.empty_like(angles)
        
        for i in range(self.num_sensors):
            end = b2Vec2(ends[i,0], ends[i,1])
            self.world.RayCast(self.raycast_callback, start, end)
            lidar_results[i] = (self.raycast_callback.point - start).length
        
        return lidar_results


class PlanningMap:
    ### 用于Planning task，你只需要知道接口用法，不需要知道实现过程 ###
    def __init__(self, walls) -> None:
        self.world = b2World(gravity=(0, 0))
        list_walls = walls.tolist()
        self.walls = [[float(wall[0]), float(wall[1])] for wall in list_walls]
        self.width = walls[-1][1]
        self.height = walls[-1][0]
        for wall in walls.tolist():
            self.world.CreateStaticBody(position=wall, shapes=b2PolygonShape(box=(0.75, 0.75)))
    
    def checkline(self, point_A, point_B):
        """
        检查从A到B是否能直接通过
        输入AB两点坐标（注意如果是numpy array需要转化为list），输出从A到B的直线上是否有障碍物
        如果有，返回True和直线与障碍物的第一个交点，如果没有，返回False和None
        """
        ###  ###
        callback = RayCastClosestCallback()
        self.world.RayCast(callback, point_A, point_B)
        hit_position = callback.point if callback.hit else None
        return callback.hit, hit_position
    
    def checkoccupy(self, point):
        """
        检查pacman的质心坐标是否能处于给定point位置上，这里提供一种最简单的用法
        如果pacman圆球与周围障碍物有穿模返回True，否则返回False
        另外可以有更优雅的写法，可以自己在answer文件里实现，具体参考https://www.zhihu.com/question/24251545/answer/27184960
        """
        point_floor = np.floor(point)
        point_remain = point - point_floor
        occupied = []
        if point_remain[0] < 0.75 and point_remain[1] < 0.75:
            occupied.append([point_floor[0], point_floor[1]])
        if point_remain[0] < 0.75 and point_remain[1] > 0.25:
            occupied.append([point_floor[0], point_floor[1] + 1])
        if point_remain[0] > 0.25 and point_remain[1] < 0.75:
            occupied.append([point_floor[0] + 1, point_floor[1]])
        if point_remain[0] > 0.25 and point_remain[1] > 0.25:
            occupied.append([point_floor[0] + 1, point_floor[1] + 1])
        for block in occupied:
            if block in self.walls:
                return True
        return False
