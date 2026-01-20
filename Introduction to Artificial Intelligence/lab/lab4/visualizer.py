import os
from direct.showbase.ShowBase import ShowBase
import numpy as np
from scipy.spatial.transform import Rotation as R
from panda3d.core import ClockObject
import panda3d.core as pc
import math
from direct.showbase.DirectObject import DirectObject
from direct.gui.DirectGui import *
from simuScene import Scene2D

import time

# from memory_profiler import profile
class CameraCtrl(DirectObject):
    def __init__(self, base, camera):
        super(CameraCtrl).__init__()
        self.accept('mouse1',self.onMouse1Down)
        self.accept('mouse1-up',self.onMouse1Up)
        self.accept('mouse2',self.onMouse2Down)
        self.accept('mouse2-up',self.onMouse2Up)
        self.accept('mouse3',self.onMouse3Down)
        self.accept('mouse3-up',self.onMouse3Up)
        self.accept('wheel_down',self.onMouseWheelDown)
        self.accept('wheel_up',self.onMouseWheelUp)

        self.accept('control-mouse1',self.onMouse1Down)
        self.accept('control-mouse1-up',self.onMouse1Up)
        self.accept('control-mouse2',self.onMouse2Down)
        self.accept('control-mouse2-up',self.onMouse2Up)
        self.accept('control-mouse3',self.onMouse3Down)
        self.accept('control-mouse3-up',self.onMouse3Up)
        self.accept('control-wheel_down',self.onMouseWheelDown)
        self.accept('control-wheel_up',self.onMouseWheelUp)

        self.position = pc.LVector3(6.83179, 48.2988, 11.3567)
        self.center = pc.LVector3(10.6287, 4.89818, 11.3222)
        self.up = pc.LVector3(0,1,0)

        self.base = base
        base.taskMgr.add(self.onUpdate, 'updateCamera', sort = 2)
        self.camera = camera
        
        self._locked_info = None
        self._locked_mouse_pos = None
        self._mouse_id = -1
        self.gamepad_x = 0
        self.gamepad_y = 0
        self.has_pad = False
        self.look()
        self._locked_info = (pc.LVector3(self.position), pc.LVector3(self.center), pc.LVector3(self.up))
        
        
    def look(self):    
        self.camera.setPos(self.position)
        self.camera.lookAt(self.center, self.up)

    @property
    def _mousePos(self):
        return pc.LVector2(self.base.mouseWatcherNode.getMouseX(), self.base.mouseWatcherNode.getMouseY())

    def _lockMouseInfo(self):
        self._locked_info = (pc.LVector3(self.position), pc.LVector3(self.center), pc.LVector3(self.up))
        self._locked_mouse_pos = self._mousePos

    def onMouse1Down(self):
        self._lockMouseInfo()
        self._mouse_id = 1

    def onMouse1Up(self):
        self._mouse_id = -1

    def onMouse2Down(self):
        self._lockMouseInfo()
        self._mouse_id = 2

    def onMouse2Up(self):
        self._mouse_id = -1

    def onMouse3Down(self):
        self._lockMouseInfo()
        self._mouse_id = 3

    def onMouse3Up(self):
        self._mouse_id = -1

    def onMouseWheelDown(self):
        z =  self.position - self.center 
        
        scale = 1.1

        if scale < 0.05:
            scale = 0.05

        self.position = self.center + z * scale
        self.look()

    def onMouseWheelUp(self):
        z =  self.position - self.center 
        
        scale = 0.9

        if scale < 0.05:
            scale = 0.05

        self.position = self.center + z * scale
        self.look()
    
    def updateGamepad(self, x, y, task):
        self.gamepad_x = x
        self.gamepad_y = y
        
        self.has_pad = self.gamepad_x**2+self.gamepad_y**2 > 0.04
        
            
    def onUpdate(self, task):
        if self._mouse_id < 0 and not self.has_pad:
            return task.cont
        
        if self.has_pad:
            mousePosOff = pc.LVector2(self.gamepad_x, self.gamepad_y) * 0.02
        else:
            mousePosOff0 = self._mousePos - self._locked_mouse_pos
            mousePosOff = self._mousePos - self._locked_mouse_pos
        
        if self._mouse_id == 1 or self.has_pad:
            if self.has_pad:
                z = self.position - self.center
            else:
                z = self._locked_info[0] - self._locked_info[1]

            zDotUp = self._locked_info[2].dot(z)
            zMap = z - self._locked_info[2] * zDotUp
            angX = math.acos(zMap.length() / z.length()) / math.pi * 180.0

            if zDotUp < 0:
                angX = -angX

            angleScale = 200.0

            x = self._locked_info[2].cross(z)
            x.normalize()
            y = z.cross(x)
            y.normalize()

            rot_x_angle = -mousePosOff.getY() * angleScale
            rot_x_angle += angX
            if rot_x_angle > 85:
                rot_x_angle = 85
            if rot_x_angle < -85:
                rot_x_angle = -85
            rot_x_angle -= angX

            rot_y = pc.LMatrix3()
            rot_y.setRotateMat(-mousePosOff.getX() * angleScale, y, pc.CS_yup_right)
            
            rot_x = pc.LMatrix3()
            rot_x.setRotateMat(-rot_x_angle, x, pc.CS_yup_right)
            if not self.has_pad:
                self.position = self._locked_info[1] + (rot_x * rot_y).xform(z)
            else:
                self.position = self.center + (rot_x * rot_y).xform(z)

        elif self._mouse_id == 2:
            z = self._locked_info[0] - self._locked_info[1]

            shiftScale = 0.5 * z.length()

            x = self._locked_info[2].cross(z)
            z.normalize()
            x.normalize()
            y = z.cross(x)

            shift = x * -mousePosOff.getX() + y* -mousePosOff.getY()
            shift *= shiftScale
            self.position = self._locked_info[0] + shift
            self.center = self._locked_info[1] + shift

        elif self._mouse_id == 3:
            z = self._locked_info[0] - self._locked_info[1]
            
            scale = 1
            scale = 1.0 + scale * mousePosOff0.getY()

            if scale < 0.05:
                scale = 0.05

            self.position = self._locked_info[1] + z * scale

        self.look()

        return task.cont

class SimpleViewer(ShowBase):
    def __init__(self, float_base = False, scene: Scene2D = None, fStartDirect=True, windowType=None):
        '''
        this is only used for my project... lots of assumptions...
        '''
        super().__init__(fStartDirect, windowType)
        self.disableMouse()
        self.scene: Scene2D = scene
        self.float_base = float_base
        
        self.camera.lookAt(0,0.9,0)
        self.setupCameraLight()
        self.camera.setHpr(0,0,0)
        
        self.setFrameRateMeter(True)
        globalClock.setMode(ClockObject.MLimited)
        globalClock.setFrameRate(60)
        
        self.load_ground()
        
        xSize = self.pipe.getDisplayWidth()
        ySize = self.pipe.getDisplayHeight()
        props = pc.WindowProperties()
        props.setSize(min(xSize-200, 1600), min(ySize-200, 1200))
        self.win.requestProperties(props)
        
        # color for links
        color = [131/255,175/255,155/255,1]
        self.tex = self.create_texture(color, 'link_tex')
        self.load_scene()
        self.add_task(self.update, 'update')
        self.update_flag = True
        self.update_food_flag = False
        
        # keys
        self.accept('space', self.receive_space)
        
        self.pre_simulation_func = None
        self.pre_update_func = None        
    
    def receive_space(self):
        self.update_flag = not self.update_flag
        self.update_food_flag = not self.update_food_flag
            
    def create_texture(self, color, name):
        img = pc.PNMImage(32,32)
        img.fill(*color[:3])
        img.alphaFill(color[3])
        tex = pc.Texture(name)
        tex.load(img)
        return tex
        
    def load_ground(self):
        self.ground = self.loader.loadModel("material/GroundScene.egg")
        self.ground.reparentTo(self.render)
        self.ground.setScale(200, 1, 200)
        self.ground.setTexScale(pc.TextureStage.getDefault(), 50, 50)
        self.ground.setPos(0, -1, 0)
        
    def setupCameraLight(self):
        # create a orbiting camera
        self.cameractrl = CameraCtrl(self, self.cam)
        self.cameraRefNode = self.camera # pc.NodePath('camera holder')
        self.cameraRefNode.setPos(0,0,0)
        self.cameraRefNode.setHpr(0,0,0)
        self.cameraRefNode.reparentTo(self.render)
        
        self.accept("v", self.bufferViewer.toggleEnable)

        self.d_lights = []
        # Create Ambient Light
        ambientLight = pc.AmbientLight('ambientLight')
        ambientLight.setColor((0.4, 0.4, 0.4, 1))
        ambientLightNP = self.render.attachNewNode(ambientLight)
        self.render.setLight(ambientLightNP)
        
        # Directional light 01
        directionalLight = pc.DirectionalLight('directionalLight1')
        directionalLight.setColor((0.4, 0.4, 0.4, 1))
        directionalLightNP = self.render.attachNewNode(directionalLight)
        directionalLightNP.setPos(10, 10, 10)
        directionalLightNP.lookAt((0, 0, 0), (0, 1, 0))

        directionalLightNP.wrtReparentTo(self.cameraRefNode)
        self.render.setLight(directionalLightNP)
        self.d_lights.append(directionalLightNP)
        
        # Directional light 02
        directionalLight = pc.DirectionalLight('directionalLight2')
        directionalLight.setColor((0.4, 0.4, 0.4, 1))
        directionalLightNP = self.render.attachNewNode(directionalLight)
        directionalLightNP.setPos(-10, 10, 10)
        directionalLightNP.lookAt((0, 0, 0), (0, 1, 0))

        directionalLightNP.wrtReparentTo(self.cameraRefNode)
        self.render.setLight(directionalLightNP)
        self.d_lights.append(directionalLightNP)
        
        
        # Directional light 03
        directionalLight = pc.DirectionalLight('directionalLight3')
        directionalLight.setColorTemperature(6500)        
        directionalLightNP = self.render.attachNewNode(directionalLight)
        directionalLightNP.setPos(0, 20, -10)
        directionalLightNP.lookAt((0, 0, 0), (0, 1, 0))

        directionalLightNP.wrtReparentTo(self.cameraRefNode)
        directionalLight.setShadowCaster(True, 2048, 2048)
        directionalLight.getLens().setFilmSize((10,10))
        directionalLight.getLens().setNearFar(0.1,300)
        
        self.render.setLight(directionalLightNP)
        self.d_lights.append(directionalLightNP)

        self.render.setShaderAuto(True)
    
    def create_wall(self, link_id, position, scale, rot):
        # create a wall block
        box = self.loader.loadModel("material/cube.bam")
        node = self.render.attachNewNode(f"wall{link_id}")
        box.reparentTo(node)
        
        # add texture
        box.setTextureOff(1)
        box.setTexture(self.tex, 1)
        box.setScale(*scale)
        
        node.setPos(self.render, *position)
        if rot is not None:
            node.setQuat(self.render, pc.Quat(*rot[[3,0,1,2]].tolist()))
        return node
    
    def create_food(self, link_id, position):
        box = self.loader.loadModel("material/ball.bam")
        node = self.render.attachNewNode(f"food{link_id}")
        box.reparentTo(node)
        
        # add texture
        box.setTextureOff(1)
        tex = self.create_texture([1,0.2,0.2,1], f"joint{link_id}_tex")
        box.setTexture(tex, 1)
        box.setScale(0.1,0.1,0.1)
        node.setPos(self.render, *position)
        return node
    
    def create_gt_food(self, link_id, position):
        box = self.loader.loadModel("material/ball.bam")
        node = self.render.attachNewNode(f"food{link_id}")
        box.reparentTo(node)
        
        # add texture
        box.setTextureOff(1)
        tex = self.create_texture([0,0,1,1], f"gt{link_id}_tex")
        box.setTexture(tex, 1)
        box.setScale(0.1,0.1,0.1)
        node.setPos(self.render, *position)
        return node
    
    def create_pacman(self, link_id, position, scale, rot):
        # box = self.loader.loadModel("material/ball.bam")
        box = self.loader.loadModel("material/scene.gltf")
        node = self.render.attachNewNode(f"pacman{link_id}")
        box.reparentTo(node)
        
        # add texture
        box.setTextureOff(1)
        tex = self.create_texture([1,1,0,1], f"joint{link_id}_tex")
        box.setTexture(tex, 1)
        box.setScale(*scale)
        
        node.setPos(self.render, *position)
        if rot is not None:
            node.setQuat(self.render, pc.Quat(*rot[[3,0,1,2]].tolist()))
        return node

    def show_axis_frame(self):
        pose = [ [1,0,0], [0,1,0], [0,0,1] ]
        color = [ [1,0,0,1], [0,1,0,1], [0,0,1,1] ]
        for i in range(3):
            box = self.loader.loadModel("material/GroundScene.egg")
            box.setScale(0.1, 0.1, 0.1)
            box.setPos(*pose[i])
            tex = self.create_texture(color[i], f"frame{i}")
            box.setTextureOff(1)
            box.setTexture(tex,1)
            box.reparentTo(self.render)
    
    def update(self, task):
        if self.pre_update_func is not None:
            self.pre_update_func()
            
        if self.update_flag:
            t0 = time.time()
            self.simulationTask()
            t1 = time.time()
            if t1 - t0 < 1/60:
                time.sleep(1/60 - (t1 - t0))
            
        if self.update_food_flag:
            self.update_foods()
            
        return task.cont
    
    def update_foods(self):
        n = min(len(self.scene.foods), len(self.foods))
        for scene_food, node_food in zip(self.scene.foods[:n], self.foods[:n]):
            node_food.setPos(self.render, *self.b2vec2np(scene_food))
            
        if n < len(self.scene.foods):
            for i, scene_food in enumerate(self.scene.foods[n:]):
                self.foods.append(self.create_food(n + i, self.b2vec2np(scene_food)))
        else:
            for node_food in self.foods[n:]:
                node_food.setPos(self.render, *[0, -100, 0])
        if self.scene.gt_food is None:
            self.gt.setPos(self.render, *[0, -100, 0])
        else:
            self.gt.setPos(self.render, *[self.scene.gt_food[0], 0.5, self.scene.gt_food[1]])
                
    def simulationTask(self):
        # print('-------')
        # print(self.cameractrl.position)
        # print(self.cameractrl.center)
        # print(self.cameractrl.up)
        self.scene.step_once()
        self.sync_viewer()
        return
    
    def sync_viewer(self):
        if self.pacman is not None:
            pos = self.b2vec2np(self.scene.pacman.position)
            quat = self.velo2quat(self.scene.pacman.linearVelocity)
            self.pacman.setPos(self.render, *pos.tolist())
            self.pacman.setQuat(self.render, pc.Quat(*quat[[3,0,1,2]].tolist()))
    
    def load_scene(self):
        groundBodies, foods = [], []
        scale = [0.5, 0.5, 0.5]
        for i in range(len(self.scene.groundBodies)):
            groundBodies.append(self.create_wall(i, self.b2vec2np(self.scene.groundBodies[i].position), scale, rot=None))
        for i in range(len(self.scene.foods)):
            foods.append(self.create_food(i, self.b2vec2np(self.scene.foods[i])))
        self.gt = self.create_gt_food(0, [0, -100, 0])
        self.groundBodies = groundBodies
        self.foods = foods
        scale = [0.25, 0.25, 0.25]
        if self.scene.pacman is not None:
            self.pacman = self.create_pacman(0, self.b2vec2np(self.scene.pacman.position), scale, rot=None)
        else:
            self.pacman = None
    
    @staticmethod
    def b2vec2np(pos_2d):
        pos = np.zeros((3,))
        pos[0] = pos_2d[0]
        pos[1] = 0.25
        pos[2] = pos_2d[1]
        return pos

    @staticmethod
    def velo2quat(vel):
        absvel = np.linalg.norm(vel) + 1e-6
        vel = vel / absvel
        angle = np.arccos(vel[0])
        if vel[1] > 0:
            angle *= -1
        return R.from_euler('xyz', np.array([0., angle, 0.])).as_quat()

    def move_marker(self, marker, x, y):
        
        if not self.update_marker_func:
            return
        
        y_axis = self.cameractrl._locked_info[2]
        z_axis = self.cameractrl.position - self.cameractrl.center
        x_axis = np.cross(y_axis, z_axis)
        x_axis = x_axis / np.linalg.norm(x_axis)
        pos = np.array(marker.getPos(self.render))
        pos += x_axis * x + y_axis * y
        marker.setPos(self.render, *pos.tolist())
        self.update_marker_func(self)
    
    def camera_fwd(self):
        return self.cameractrl.center - self.cameractrl.position
    
    def create_marker(self, pos, color):
        self.update_marker_func = None
        marker = self.loader.loadModel("material/GroundScene.egg")
        marker.setScale(0.05,0.05,0.05)
        marker.setPos(*pos)
        tex = self.create_texture(color, "marker")
        marker.setTextureOff(1)
        marker.setTexture(tex,1)
        
        marker.wrtReparentTo(self.render)
        
        self.accept('w', self.move_marker, [marker, 0, 0.05])
        self.accept('s', self.move_marker, [marker, 0, -0.05])
        self.accept('a', self.move_marker, [marker, -0.05, 0])
        self.accept('d', self.move_marker, [marker, 0.05, 0])
        
        self.accept('w-repeat', self.move_marker, [marker, 0, 0.05])
        self.accept('s-repeat', self.move_marker, [marker, 0, -0.05])
        self.accept('a-repeat', self.move_marker, [marker, -0.05, 0])
        self.accept('d-repeat', self.move_marker, [marker, 0.05, 0])
        return marker


class TestEnv:
    def __init__(self, scene: Scene2D) -> None:
        self.scene = scene
    
    def update(self):
        self.scene.step_once()
    
    def run(self):
        while self.scene.run:
            self.update()
