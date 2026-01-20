import numpy as np
np.random.seed(3407)
from typing import List
from loadMap import tryToLoad
from simuScene import Scene2D
from answerLocalization import *
from utils import Particle
import argparse


def readArgparse():
    parser = argparse.ArgumentParser()
    parser.add_argument('--test', action='store_true')
    parser.add_argument('--test_idx', type=int, default=0)
    args = parser.parse_args()
    return args


class MontoCarloLocalization:
    def __init__(self, scene: Scene2D, num_samples: int, odometry: np.array, lidar_gt: np.array) -> None:
        self.scene = scene
        self.odometry = odometry
        self.lidar_gt = lidar_gt
        self.particles: List[Particle] = []
        self.num_samples = num_samples
    
    def generate_uniform_particles(self):
        generated_particles = generate_uniform_particles(self.scene.walls, self.num_samples)
        assert len(generated_particles) == self.num_samples
        self.particles = generated_particles

    #@profile
    def __calc_particle_weights(self, iter: int):
        for p in self.particles:
            lidar_result = self.scene.lidar_sensor(p.position, p.theta)
            lidar_gt = self.lidar_gt[iter]
            p.weight = calculate_particle_weight(lidar_result, lidar_gt)
        self.__normalize_paritcle_weights()
        pass

    def __resample_particles(self):
        resampled_particles = resample_particles(self.scene.walls, self.particles)
        assert len(resampled_particles) == self.num_samples
        self.particles = resampled_particles

    def __normalize_paritcle_weights(self):
        """
        Normalize weights of particles to satisfy: sum(weights) = 1.0
        """
        cumsum = 0.0
        for p in self.particles:
            cumsum += p.weight
        for p in self.particles:
            p.weight /= (cumsum + 1e-6)
    
    def __get_odometry_update(self, iter: int):
        distance = np.linalg.norm(self.odometry[iter, :2] - self.odometry[iter-1, :2])
        dtheta = self.odometry[iter, 2] - self.odometry[iter-1, 2]
        return distance, dtheta
    
    def __get_estimate_result(self):
        return get_estimate_result(self.particles)
    
    #@profile
    def run_localization_gen(self):
        self.generate_uniform_particles()
        self.__calc_particle_weights(iter=0)
        ### Particle Filter Main Loop ###
        for i in range(1, self.lidar_gt.shape[0]):
            # 按weight将particles从大到小排序
            self.particles.sort(key=Particle.get_weight, reverse=True)
            self.__resample_particles()
            
            yield self.particles
            
            distance, dtheta = self.__get_odometry_update(iter=i)
            for p in self.particles:
                p = apply_state_transition(p, distance, dtheta)
            self.__calc_particle_weights(i)
        self.particles.sort(key=Particle.get_weight, reverse=True)
        return self.__get_estimate_result()
    
    def run_localization(self):
        gen = self.run_localization_gen()
        try:
            while True:
                _ = next(gen)
        except StopIteration as e:
            return e.value
        
def load_scene_and_run_gen(scene):
    data_num = 0 # 自己改来测试
    odometry = np.load(f'data_q1/odom_{data_num}.npy')
    lidar = np.load(f'data_q1/lidar_{data_num}.npy')
    localizer = MontoCarloLocalization(scene, 500, odometry, lidar)        
    gen = localizer.run_localization_gen()
    step_cnt = 0
    try:
        while True:
            step_cnt += 1
            particles = next(gen)
            scene.foods = [p.position for p in particles]
            scene.gt_food = localizer.odometry[step_cnt-1][:2]
            yield step_cnt
            
    except StopIteration as e:
        result_particle = e.value                        
        error = np.sqrt((result_particle.position[0]-odometry[-1, 0])**2
                        +(result_particle.position[1]-odometry[-1, 1])**2
                        +5*(result_particle.theta-odometry[-1, 2])**2)
    
    print(f"Error is {error:.6f}")
    return error

if __name__ == "__main__":
    args = readArgparse()
    layout_name = 'layouts/office.lay'
    scene = Scene2D(tryToLoad(layout_name))
    
    if not args.test:
        from visualizer import SimpleViewer
        viewer = SimpleViewer(True, scene)
        viewer.update_flag = False
        viewer.update_food_flag = True

        gen = load_scene_and_run_gen(scene)
        def pre_update_func():
            try:
                _ = next(gen)
            except StopIteration as e:
                sum_error = e.value            
                viewer.pre_update_func = None

        viewer.pre_update_func = pre_update_func
        viewer.run()
        
    else:
        odometry = np.load(f'data_q1/odom_{args.test_idx}.npy')
        lidar = np.load(f'data_q1/lidar_{args.test_idx}.npy')
        localizer = MontoCarloLocalization(scene, 500, odometry, lidar)
        result_particle = localizer.run_localization()
        x = np.abs(result_particle.theta - odometry[-1, 2]) / (2 * np.pi)
        x -= int(x)
        dtheta = 2 * np.pi * x
        dtheta = 2 * np.pi - dtheta if dtheta > np.pi else dtheta
        error = np.sqrt((result_particle.position[0]-odometry[-1, 0])**2+(result_particle.position[1]-odometry[-1, 1])**2+5*(dtheta)**2)
        print(f"Error is {error:.6f}")