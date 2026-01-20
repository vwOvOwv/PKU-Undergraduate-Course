import torch
import numpy as np
from models.vgg import SpikingVGG
from models.resnet import SpikingResNet

in_channels_dict = {
    'CIFAR10': 3,
    'CIFAR10DVS': 2,
}

num_classes_dict = {
    'CIFAR10': 10,
    'CIFAR10DVS': 10,
}

def build_model(config):
    if config['arch'].lower() == 'vgg':
        return build_vgg(config)
    elif config['arch'].lower() == 'resnet':
        return build_resnet(config)
    else:
        raise NotImplementedError(f"{config['arch']}")

def build_vgg(config):
    num_layers = config['num_layers']
    dataset = config['dataset']
    light_classifier = config['light_classifier']
    dropout = config['dropout']
    in_channels = in_channels_dict[dataset]
    num_classes = num_classes_dict[dataset]
    has_temporal_dim = True if 'dvs' in dataset.lower() else False
    T = config['T']
    surrogate = config['surrogate']
    alpha = config['alpha']

    return SpikingVGG(num_layers, num_classes, in_channels, T, surrogate, 
                      alpha, dropout, light_classifier, has_temporal_dim)

def build_resnet(config):
    num_layers = config['num_layers']
    dataset = config['dataset']
    in_channels = in_channels_dict[dataset]
    num_classes = num_classes_dict[dataset]
    has_temporal_dim = True if 'DVS' in dataset else False
    T = config['T']
    surrogate = config['surrogate']
    alpha = config['alpha']
    zero_init_residual = config['zero_init_residual']

    return SpikingResNet(num_layers, num_classes, in_channels, T, surrogate, 
                         alpha, zero_init_residual, has_temporal_dim)