import torch
import torch.nn as nn
import numpy as np
from modules.block import SpikingBasicBlock, SpikingBottleneck
from modules.activation import LIF
from modules.dim import AddTemporalDim, MergeTemporalDim, SplitTemporalDim
from typing import Any

__all__ = [
    'SpikingResNet'
]

standard_cfg = {
    18: {'block': SpikingBasicBlock, 'layers': [2, 2, 2, 2]},
    34: {'block': SpikingBasicBlock, 'layers': [3, 4, 6, 3]},
    50: {'block': SpikingBottleneck, 'layers': [3, 4, 6, 3]},
    101: {'block': SpikingBottleneck, 'layers': [3, 4, 23, 3]},
    152: {'block': SpikingBottleneck, 'layers': [3, 8, 16, 3]}
}

class SpikingResNet(nn.Module):
    def __init__(self, num_layers, num_classes, in_channels, T, surrogate, 
                 alpha, zero_init_residual, has_temporal_dim):
        super(SpikingResNet, self).__init__()
        self.in_channels = 64

        if not has_temporal_dim:
            self.init_expand = AddTemporalDim(T)
        else:
            self.init_expand = nn.Identity()
        self.init_merge = MergeTemporalDim(T)
        self.conv1 = nn.Conv2d(in_channels, self.in_channels, kernel_size=3, stride=1, padding=1, bias=False)
        self.bn1 = nn.BatchNorm2d(self.in_channels)
        self.split1 = SplitTemporalDim(T)
        self.lif1 = LIF(surrogate_function=surrogate, alpha=alpha)
        self.merge1 = MergeTemporalDim(T)
        # self.maxpool = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        self.maxpool = nn.Identity()
        
        cfg = standard_cfg[num_layers]
        block = cfg['block']
        layers = cfg['layers']

        self.layer1 = self._make_layers(block, surrogate, alpha, T,  64, layers[0], stride=1)
        self.layer2 = self._make_layers(block, surrogate, alpha, T, 128, layers[1], stride=2)
        self.layer3 = self._make_layers(block, surrogate, alpha, T, 256, layers[2], stride=2)
        self.layer4 = self._make_layers(block, surrogate, alpha, T, 512, layers[3], stride=2)

        self.adaptive_avgpool = nn.AdaptiveAvgPool2d((1, 1))
        self.fc = nn.Linear(512 * block.expansion, num_classes)
        self.final_split = SplitTemporalDim(T)

        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                nn.init.kaiming_normal_(m.weight, mode='fan_out', nonlinearity='relu')
            elif isinstance(m, nn.BatchNorm2d):
                nn.init.constant_(m.weight, 1)
                nn.init.constant_(m.bias, 0)
        if zero_init_residual:
            for m in self.modules():
                if isinstance(m, SpikingBottleneck):
                    nn.init.constant_(m.bn3.weight, 0)
                elif isinstance(m, SpikingBasicBlock):
                    nn.init.constant_(m.bn2.weight, 0)

    def _make_layers(self, block, surrogate, alpha, T, out_channels, num_blocks, stride=1):
        downsample = None
        if stride != 1 or self.in_channels != out_channels * block.expansion:
            downsample = nn.Sequential(
                nn.Conv2d(self.in_channels, out_channels * block.expansion, kernel_size=1, stride=stride, bias=False),
                nn.BatchNorm2d(out_channels * block.expansion),
                SplitTemporalDim(T),
                LIF(surrogate_function=surrogate, alpha=alpha),
                MergeTemporalDim(T)
            )

        layers = []
        layers.append(block(self.in_channels, out_channels, surrogate, alpha, T, stride, downsample))
        
        self.in_channels = out_channels * block.expansion
        
        for _ in range(1, num_blocks):
            layers.append(block(self.in_channels, out_channels, surrogate, alpha, T))

        return nn.Sequential(*layers)

    def forward(self, x):
        x = self.init_expand(x)
        x = self.init_merge(x)
        x = self.conv1(x)
        x = self.bn1(x)
        x = self.split1(x)
        x = self.lif1(x)
        x = self.merge1(x)
        x = self.maxpool(x)

        x = self.layer1(x)
        x = self.layer2(x)
        x = self.layer3(x)
        x = self.layer4(x)

        x = self.adaptive_avgpool(x)
        x = torch.flatten(x, 1)
        x = self.fc(x)
        x = self.final_split(x)

        return x.mean(dim=0)