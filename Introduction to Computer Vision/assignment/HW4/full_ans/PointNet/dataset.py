from __future__ import print_function
import torch.utils.data as data
import os
import os.path
import torch
import numpy as np
from tqdm import tqdm 
import json


def data_augmentation(point_set):
    theta = np.random.uniform(0,np.pi*2)
    rotation_matrix = np.array([[np.cos(theta), -np.sin(theta)],[np.sin(theta), np.cos(theta)]])
    point_set[:,[0,2]] = point_set[:,[0,2]].dot(rotation_matrix) # random rotation
    point_set += np.random.normal(0, 0.02, size=point_set.shape) # random jitter
    return point_set



class ShapeNetClassficationDataset(data.Dataset):
    def __init__(self,
                    root,
                    npoints=2500,
                    classification=False,
                    class_choice=None,
                    split='train',
                    with_data_augmentation=True):
            self.npoints = npoints
            self.root = root
            self.catfile = os.path.join(self.root, 'synsetoffset2category.txt')
            self.cat = {}
            self.with_data_augmentation = with_data_augmentation
            self.classification = classification
            
            with open(self.catfile, 'r') as f:
                for line in f:
                    ls = line.strip().split()
                    self.cat[ls[0]] = ls[1]

            if not class_choice is None:
                self.cat = {k: v for k, v in self.cat.items() if k in class_choice}

            self.id2cat = {v: k for k, v in self.cat.items()}

            self.meta = {}
            splitfile = os.path.join(self.root, 'train_test_split', 'shuffled_{}_file_list.json'.format(split))
            #from IPython import embed; embed()
            filelist = json.load(open(splitfile, 'r'))
            for item in self.cat:
                self.meta[item] = []

            for file in filelist:
                _, category, uuid = file.split('/')
                if category in self.cat.values():
                    self.meta[self.id2cat[category]].append((os.path.join(self.root, category, 'points', uuid+'.pts'),
                                            os.path.join(self.root, category, 'points_label', uuid+'.seg')))

            self.datapath = []
            for item in self.cat:
                for fn in self.meta[item]:
                    self.datapath.append((item, fn[0], fn[1]))

            self.classes = dict(zip(sorted(self.cat), range(len(self.cat))))
            # print(self.classes)


    def __getitem__(self, index):
        fn = self.datapath[index]
        # print("fn",fn)
        cls = self.classes[self.datapath[index][0]]
        # print("cls",cls)
        point_set = np.loadtxt(fn[1]).astype(np.float32)

        choice = np.random.choice(point_set.shape[0], self.npoints, replace=True)
        #resample
        point_set = point_set[choice, :]

        point_set = point_set - np.expand_dims(np.mean(point_set, axis = 0), 0) # center
        dist = np.max(np.sqrt(np.sum(point_set ** 2, axis = 1)),0)
        point_set = point_set / dist #scale

        if self.with_data_augmentation:
            point_set = data_augmentation(point_set)
        point_set = torch.from_numpy(point_set)
        cls = torch.from_numpy(np.array([cls]).astype(np.int64))

        return point_set, cls


    def __len__(self):
        return len(self.datapath)





class ShapeNetSegmentationDataset(data.Dataset):
    def __init__(self,
                 root,
                 npoints=2500,
                 class_choice=None,
                 split='train',
                 with_data_augmentation=True):
        self.npoints = npoints
        self.root = root
        self.catfile = os.path.join(self.root, 'synsetoffset2category.txt')
        self.cat = {}
        self.seg_classes = {}
        self.with_data_augmentation=with_data_augmentation
        with open(self.catfile, 'r') as f:
            for line in f:
                ls = line.strip().split()
                self.cat[ls[0]] = ls[1]
        #print(self.cat)
        if not class_choice is None:
            self.cat = {k: v for k, v in self.cat.items() if k in class_choice}

        self.id2cat = {v: k for k, v in self.cat.items()}

        self.meta = {}
        splitfile = os.path.join(self.root, 'train_test_split', 'shuffled_{}_file_list.json'.format(split))
        #from IPython import embed; embed()
        filelist = json.load(open(splitfile, 'r'))
        for item in self.cat:
            self.meta[item] = []

        for file in filelist:
            _, category, uuid = file.split('/')
            if category in self.cat.values():
                self.meta[self.id2cat[category]].append((os.path.join(self.root, category, 'points', uuid+'.pts'),
                                        os.path.join(self.root, category, 'points_label', uuid+'.seg')))

        self.datapath = []
        for item in self.cat:
            for fn in self.meta[item]:
                self.datapath.append((item, fn[0], fn[1]))

        self.classes = dict(zip(sorted(self.cat), range(len(self.cat))))
        # print(self.classes)
        with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'num_seg_classes.txt'), 'r') as f:
            for line in f:
                ls = line.strip().split()
                self.seg_classes[ls[0]] = int(ls[1])
        self.num_seg_classes = self.seg_classes[list(self.cat.keys())[0]]
        # print(self.seg_classes, self.num_seg_classes)

    def __getitem__(self, index):
        fn = self.datapath[index]
        point_set = np.loadtxt(fn[1]).astype(np.float32)
        seg = np.loadtxt(fn[2]).astype(np.int64)

        choice = np.random.choice(point_set.shape[0], self.npoints, replace=True)
        #resample
        point_set = point_set[choice, :]

        point_set = point_set - np.expand_dims(np.mean(point_set, axis = 0), 0) # center
        dist = np.max(np.sqrt(np.sum(point_set ** 2, axis = 1)),0)
        point_set = point_set / dist #scale

        if self.with_data_augmentation:
            point_set = data_augmentation(point_set)


        seg = seg[choice]
        point_set = torch.from_numpy(point_set)
        seg = torch.from_numpy(seg)


        return point_set, seg

    def __len__(self):
        return len(self.datapath)





