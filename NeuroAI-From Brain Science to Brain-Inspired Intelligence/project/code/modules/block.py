import torch
import torch.nn as nn
import numpy as np
from modules.activation import LIF
from modules.dim import MergeTemporalDim, SplitTemporalDim
from typing import Any

class SpikingBasicBlock(nn.Module):
    expansion = 1

    def __init__(self, in_channels, out_channels, surrogate, alpha, T, stride=1, downsample=None):
        super(SpikingBasicBlock, self).__init__()
        
        self.conv1 = nn.Conv2d(in_channels, out_channels, kernel_size=3, stride=stride, padding=1, bias=False)
        self.bn1 = nn.BatchNorm2d(out_channels)
        self.split1 = SplitTemporalDim(T)
        self.lif1 = LIF(surrogate_function=surrogate, alpha=alpha)
        self.merge1 = MergeTemporalDim(T)
        
        self.conv2 = nn.Conv2d(out_channels, out_channels, kernel_size=3, stride=1, padding=1, bias=False)
        self.bn2 = nn.BatchNorm2d(out_channels)
        self.split2 = SplitTemporalDim(T)
        self.lif2 = LIF(surrogate_function=surrogate, alpha=alpha)
        self.merge2 = MergeTemporalDim(T)
        
        self.downsample = downsample

    def forward(self, x):
        identity = x

        out = self.conv1(x)
        out = self.bn1(out)
        out = self.split1(out)
        out = self.lif1(out)
        out = self.merge1(out)

        out = self.conv2(out)
        out = self.bn2(out)
        out = self.split2(out)
        out = self.lif2(out)
        out = self.merge2(out)

        if self.downsample is not None:
            identity = self.downsample(x)
        out += identity

        return out
    

class SpikingBottleneck(nn.Module):
    expansion = 4

    def __init__(self, in_channels, out_channels, surrogate, alpha, T, stride=1, downsample=None):
        super(SpikingBottleneck, self).__init__()
        
        self.conv1 = nn.Conv2d(in_channels, out_channels, kernel_size=1, stride=1, bias=False)
        self.bn1 = nn.BatchNorm2d(out_channels)
        self.split1 = SplitTemporalDim(T)
        self.lif1 = LIF(surrogate_function=surrogate, alpha=alpha)
        self.merge1 = MergeTemporalDim(T)
        
        self.conv2 = nn.Conv2d(out_channels, out_channels, kernel_size=3, stride=stride, padding=1, bias=False)
        self.bn2 = nn.BatchNorm2d(out_channels)
        self.split2 = SplitTemporalDim(T)
        self.lif2 = LIF(surrogate_function=surrogate, alpha=alpha)
        self.merge2 = MergeTemporalDim(T)
        
        self.conv3 = nn.Conv2d(out_channels, out_channels * self.expansion, kernel_size=1, stride=1, bias=False)
        self.bn3 = nn.BatchNorm2d(out_channels * self.expansion)
        self.split3 = SplitTemporalDim(T)
        self.lif3 = LIF(surrogate_function=surrogate, alpha=alpha)
        self.merge3 = MergeTemporalDim(T)
        
        self.downsample = downsample

    def forward(self, x):
        identity = x

        out = self.conv1(x)
        out = self.bn1(out)
        out = self.split1(out)
        out = self.lif1(out)
        out = self.merge1(out)

        out = self.conv2(out)
        out = self.bn2(out)
        out = self.split2(out)
        out = self.lif2(out)
        out = self.merge2(out)

        out = self.conv3(out)
        out = self.bn3(out)

        if self.downsample is not None:
            identity = self.downsample(x)

        out = self.split3(out)
        out = self.lif3(out)
        out = self.merge3(out)
        out += identity

        return out