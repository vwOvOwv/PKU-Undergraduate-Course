import cv2
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

def read_img(path):
    return cv2.imread(path, cv2.IMREAD_GRAYSCALE)

def write_img(path, img):
    cv2.imwrite(path, img)

def draw_corner(img_path, save_path, coner_list):
    vis_img = cv2.imread(img_path)
    for point in coner_list:
        cv2.circle(vis_img, (int(point[1]), int(point[0])), 2, (0,0,255), -1)

    write_img(save_path, vis_img)


def plane_func(pf, p_xy):
    return (pf[0]*p_xy[:,0] + pf[1]*p_xy[:,1] + pf[3])/-pf[2] 

def draw_save_plane_with_points(estimated_pf, p, path): # plane function: pf[0]*x+pf[1]*y+pf[2]*z+pf[3]=0
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    ax.scatter(p[:,0], p[:,1], p[:,2], c="g", s=10)
    x = np.linspace(-1, 1, 100)
    y = np.linspace(-1, 1, 100)
    x, y = np.meshgrid(x, y)
    estimated_plane_z = plane_func(estimated_pf, np.concatenate((x.reshape(-1,1), y.reshape(-1,1)),axis=1))
    ax.plot_surface(x, y, estimated_plane_z.reshape(100, 100), alpha=0.5)
    ax.view_init(elev=45, azim=315)
    plt.savefig(path)
    plt.show()

def normalize(pf):
    return pf / np.linalg.norm(pf) * np.sign(pf[0])

    