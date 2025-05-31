from __future__ import print_function
import os
import random
import torch
import torch.nn.parallel
import torch.utils.data
from dataset import ShapeNetClassficationDataset
from model import PointNetCls1024D
import numpy as np
from utils import write_points, setting
import cv2
import matplotlib.pyplot as plt

if __name__ == '__main__':
    feat_dim = 1024
    batch_size = 16
    num_classes = 16
    opt = setting()
    blue = lambda x: '\033[94m' + x + '\033[0m'
    random.seed(opt.manualSeed)
    torch.manual_seed(opt.manualSeed)


    test_dataset = ShapeNetClassficationDataset(
        root=opt.dataset,
        split='test',
        npoints=opt.num_points,
        class_choice=['Airplane', 'Lamp', 'Guitar', 'Laptop'],
        with_data_augmentation=False)
   

    testdataloader = torch.utils.data.DataLoader(
            test_dataset,
            batch_size=batch_size,
            shuffle=True,
            num_workers=int(opt.workers))

    print('classes', num_classes)


    classifier = PointNetCls1024D(k=num_classes)

    # load weights:
    classifier.load_state_dict(torch.load(f"{opt.expf}/cls_{feat_dim}D/model.pth"))

    classifier.eval()

    for i,data in enumerate(testdataloader, 0):
        points, target = data
        target = target[:, 0]
        classifier = classifier.eval()

        pred, heat_feat = classifier(points)
        heat_feat = heat_feat.detach().numpy()
        heat_feat = np.max(heat_feat, 2)
        heat_feat = (heat_feat - np.min(heat_feat,axis=0))/(np.max(heat_feat,axis=0) - np.min(heat_feat,axis=0))
        color_heat_feat = cv2.applyColorMap((heat_feat*255).astype(np.uint8), cv2.COLORMAP_JET) #BGR

        for i in range(batch_size):
            fig = plt.figure()
            ax = fig.add_subplot(111, projection='3d')

            point = points.numpy()[i,...]
            ax.scatter(point[:, 0], point[:, 1], point[:, 2], c=color_heat_feat[i,...]/255, marker='o')

            ax.set_xlim(-1, 1)
            ax.set_ylim(-1, 1)
            ax.set_zlim(-1, 1)

            plt.show()
        
        break
