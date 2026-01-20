import numpy as np
np.random.seed(3407)
import argparse
from loadMap import tryToLoad
from simuScene import Scene2D
from visualizer import SimpleViewer, TestEnv
from answerPDControl import calc_pd_force
from physicsWrapper import PhysicsInfo


def readArgparse():
    parser = argparse.ArgumentParser()
    parser.add_argument('--test', action='store_true')
    parser.add_argument('--test_idx', type=int, default=0)
    args = parser.parse_args()
    return args


class PDController:
    def __init__(self, scene: Scene2D, food_idx: int) -> None:
        self.scene = scene
        self.food_idx = food_idx
        self.physice_info = PhysicsInfo(scene)
        self.cnt = 0
        self.checkpoint = None
        self.arrive = False
        self.kp = 10
        self.kd = 1
    
    def get_target(self):
        return self.scene.foods[self.food_idx]
    
    def apply_pd_force(self):
        target_pose = self.get_target()
        force = calc_pd_force(target_pose, self.physice_info.pacman_position(), 
                              self.physice_info.pacman_velocity(),
                              kp=self.kp, kd=self.kd)
        self.scene.apply_force(force)
    
    def check_stop(self):
        target_pose = self.get_target()
        current_pos = self.physice_info.pacman_position()
        if self.cnt == 60:
            self.scene.checkpoint = current_pos
        if self.cnt == 180:
            self.scene.arrive = self.scene.check_eat(self.food_idx)
            self.scene.run = False
        self.cnt += 1

def task_pd_control(scene: Scene2D, food_idx):
    controller = PDController(scene, food_idx)
    scene.pre_simulation_func = controller.apply_pd_force
    scene.post_simulation_func = controller.check_stop


if __name__ == "__main__":
    args = readArgparse()
    layout_name = 'layouts/empty.lay'
    scene = Scene2D(tryToLoad(layout_name))
    if not args.test:
        task_pd_control(scene, food_idx=9) # 这里food_idx你可以自己改来看target距离不同时controller的表现
        viewer = SimpleViewer(True, scene)
        viewer.run()
    else:
        task_pd_control(scene, food_idx=args.test_idx)
        testenv = TestEnv(scene)
        testenv.run()
        print(f"{scene.arrive} {scene.checkpoint[0]} {scene.checkpoint[1]}")
