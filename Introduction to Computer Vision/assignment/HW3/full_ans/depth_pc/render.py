# This code is used to render a depth image from a mesh.
# This code is only provided for interested readers and you don't have to run this.

import pyrender
import trimesh 
import numpy as np 
import cv2 

# # if you are using a headless server, you may need below lines
# import os
# os.environ['PYOPENGL_PLATFORM'] = 'egl'

# load object and preprocess
mesh = trimesh.load('spot.obj', force='mesh')
base_scale = np.sqrt(((mesh.vertices.max(axis=0)-mesh.vertices.min(axis=0))**2).sum())
obj_center = np.array(mesh.vertices.max(axis=0) + mesh.vertices.min(axis=0))
mesh.vertices = (mesh.vertices - obj_center) / base_scale
mesh.vertices = mesh.vertices + [0.5,0.5,-1]
np.savetxt('raw_full_pc.txt', np.array(mesh.vertices))

# pyrender load object
scene = pyrender.Scene()
obj_mesh = pyrender.Mesh.from_trimesh(mesh)
obj_node = pyrender.Node(mesh=obj_mesh, matrix=np.eye(4))
scene.add_node(obj_node)

# initialize camera
pw = 640
ph = 480
camera_pose = np.eye(4)
camera = pyrender.PerspectiveCamera(yfov=np.deg2rad(60), aspectRatio=pw / ph, znear=0.1, zfar=10)
scene.add(camera, camera_pose)

# render
r = pyrender.OffscreenRenderer(pw, ph)
seg_img, depth = r.render(scene, flags=pyrender.constants.RenderFlags.SEG, seg_node_map={obj_node:[255,0,0]})

depth_scale = 0.00012498664727900177
print(depth.mean(), depth.max(), depth.min())

depth = (depth / depth_scale).astype(np.int32)
depth_img = np.zeros_like(seg_img)
depth_img[..., 1] = depth // 256
depth_img[..., 2] = depth % 256

cv2.imwrite('seg.png', seg_img)
cv2.imwrite('depth.png', depth_img)


# intrinsic
projection = camera.get_projection_matrix()
K = np.eye(3)
K[0,0] = projection[0,0] * pw / 2
K[1,1] = projection[1,1] * ph / 2
K[0,2] = pw / 2
K[1,2] = ph / 2
np.save('intrinsic', K)