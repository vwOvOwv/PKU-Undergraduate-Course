import os
import numpy as np


class Layout:
    def __init__(self, layoutText=None) -> None:
        self.height = 0
        self.width = 0
        self.walls = []
        self.pacman_pos = None
        self.foods = []
        if layoutText is not None:
            self.load_layout(layoutText)
    
    def load_layout(self, layoutText) -> None:
        self.height = len(layoutText)
        self.width = 0
        for y in range(self.height):
            self.width = max(self.width, len(layoutText[y]))
            for x in range(len(layoutText[y])):
                self.processLayoutChar(y, x, layoutText[y][x])
    
    def processLayoutChar(self, x, y, layoutChar):
        if layoutChar == '%':
            self.walls.append((x, y))
        elif layoutChar == '.':
            self.foods.append(np.array([x, y]))
        elif layoutChar == 'P':
            self.pacman_pos = (x, y)
        elif layoutChar == ' ':
            pass
        else:
            raise NotImplementedError
    
    def get_empty_map(self):
        new_map = Layout()
        new_map.height = self.height
        new_map.width = self.width
        new_map.walls = self.walls
        return new_map
        

def tryToLoad(fullname):
    if(not os.path.exists(fullname)): return None
    f = open(fullname)
    try: return Layout([line.strip() for line in f])
    finally: f.close()