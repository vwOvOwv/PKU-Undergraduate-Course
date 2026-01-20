import os
import torchvision.datasets
import torchvision.transforms as transforms
from torchtoolbox.transform import Cutout
from torch.utils.data import Dataset
import torch
from spikingjelly.datasets.cifar10_dvs import CIFAR10DVS
from spikingjelly.datasets import split_to_train_test_set
import numpy as np

def load_cifar10(data_path):
    print("loading CIFAR10")
    if not os.path.exists(data_path):
        os.mkdir(data_path)
    transform_train = transforms.Compose([
            transforms.RandomCrop(32, padding=4),
            Cutout(),
            transforms.RandomHorizontalFlip(),
            transforms.ToTensor(),
        ])

    transform_val = transforms.Compose([
            transforms.ToTensor(),
        ])

    train_set = torchvision.datasets.CIFAR10(data_path, train=True, transform=transform_train, download=True)
    val_set = torchvision.datasets.CIFAR10(data_path, train=False, transform=transform_val, download=True)
    
    return train_set, val_set

class PackagingClass(torch.utils.data.Dataset):
    def __init__(self, dataset, transform=None):
        self.transform = transform
        self.dataset = dataset

    def __getitem__(self, index):
        data, label = self.dataset[index]
        data = torch.FloatTensor(data)
        if self.transform:
            data = self.transform(data)
        return data, label

    def __len__(self):
        return len(self.dataset)


class DvsCutout(object):
    def __init__(self, length):
        self.length = length

    def __call__(self, img):
        h = img.size(2)
        w = img.size(3)
        mask = np.ones((h, w), np.float32)
        y = np.random.randint(h)
        x = np.random.randint(w)
        y1 = np.clip(y - self.length // 2, 0, h)
        y2 = np.clip(y + self.length // 2, 0, h)
        x1 = np.clip(x - self.length // 2, 0, w)
        x2 = np.clip(x + self.length // 2, 0, w)
        mask[y1: y2, x1: x2] = 0.
        mask = torch.from_numpy(mask)
        mask = mask.expand_as(img)
        img = img * mask
        return img


def function_nda(data, M=1, N=2):
    c = 15 * N
    rotate_tf = transforms.RandomRotation(degrees=c)
    e = 8 * N
    cutout_tf = DvsCutout(length=e)

    def roll(data, N=1):
        a = N * 2 + 1
        off1 = np.random.randint(-a, a + 1)
        off2 = np.random.randint(-a, a + 1)
        return torch.roll(data, shifts=(off1, off2), dims=(2, 3))

    def rotate(data, N):
        return rotate_tf(data)

    def cutout(data, N):
        return cutout_tf(data)

    transforms_list = [roll, rotate, cutout]
    sampled_ops = np.random.choice(transforms_list, M)
    for op in sampled_ops:
        data = op(data, N)
    return data

def load_cifar10dvs(data_path, T):
    if not os.path.exists(data_path):
        os.mkdir(data_path)

    def transform_train(data):
        data = transforms.RandomResizedCrop(128, scale=(0.7, 1.0), interpolation=transforms.InterpolationMode.NEAREST)(data)
        resize = transforms.Resize(size=(48, 48))
        data = resize(data).float()
        flip = np.random.random() > 0.5
        if flip:
            data = torch.flip(data, dims=(3,))
        data = function_nda(data)
        return data.float()

    def transform_val(data):
        resize = transforms.Resize(size=(48, 48))
        data = resize(data).float()
        return data.float()
    
    dataset = CIFAR10DVS(data_path, data_type='frame', frames_number=T, split_by='number')
    train_set, val_set = split_to_train_test_set(train_ratio=0.9, origin_dataset=dataset, num_classes=10)
    train_set, val_set = PackagingClass(train_set, transform_train), PackagingClass(val_set, transform_val)
    return train_set, val_set