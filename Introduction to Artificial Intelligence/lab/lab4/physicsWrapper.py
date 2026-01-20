import numpy as np
from simuScene import Scene2D

class PhysicsInfo:
    def __init__(self, scene: Scene2D) -> None:
        self.__scene = scene
    
    def pacman_position(self):
        return np.array(self.__scene.pacman.position)

    def pacman_velocity(self):
        return np.array(self.__scene.pacman.linearVelocity)