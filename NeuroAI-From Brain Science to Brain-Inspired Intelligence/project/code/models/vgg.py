import torch.nn as nn
from modules.activation import LIF
from modules.dim import AddTemporalDim, MergeTemporalDim, SplitTemporalDim

__all__ = [
    'SpikingVGG'
]

cfg = {
    8:  [64, 'P', 128, 'P', 256, 'P', 512, 'P', 512, 'P'],
    11: [64, 'P', 128, 'P', 256, 256, 'P', 512, 512, 'P', 512, 512, 'P'],
    16: [64, 64, 'P', 128, 128, 'P', 256, 256, 256, 'P', 
         512, 512, 512, 'P', 512, 512, 512, 'P'],
    19: [64, 64, 'P', 128, 128, 'P', 256, 256, 256, 256, 'P', 
         512, 512, 512, 512, 'P', 512, 512, 512, 512, 'P'],
}


class SpikingVGG(nn.Module):
    def __init__(self, num_layers, num_classes, in_channels, T, 
                 surrogate, alpha, dropout, light_classifier, has_temporal_dim):
        super().__init__()
        self.num_layers = num_layers
        self.num_classes = num_classes
        self.in_channels = in_channels
        self.T = T
        self.surrogate = surrogate
        self.alpha = alpha
        self.light_classifier = light_classifier
        self.has_temporal_dim = has_temporal_dim
        self.dropout = dropout
        self.conv_config = {
            'kernel_size': 3, 'stride': 1, 'padding': 1, 'dilation': 1,
            'groups': 1, 'padding_mode': 'zeros', 'bias': False
        }

        self.layers = self._build_model(cfg[self.num_layers])
        
    def _build_model(self, cfg):
        layers = []
        if not self.has_temporal_dim:
            layers.append(AddTemporalDim(self.T))
        layers.append(MergeTemporalDim(self.T))
        layers += self._make_extractor(cfg)
        layers += self._make_classifier(cfg[-2])
        return nn.Sequential(*layers)

    def _make_extractor(self, cfg):
        """
        Construct feature extractor.
        """
        layers = []
        in_channels = self.in_channels
        for i in range(len(cfg)):
            if cfg[i] == 'P':
                layers.append(nn.AvgPool2d(2, 2, 0))
            else:
                layers.append(nn.Conv2d(in_channels, cfg[i], **self.conv_config))
                layers.append(nn.BatchNorm2d(cfg[i]))
                layers.append(SplitTemporalDim(self.T))
                layers.append(LIF(surrogate_function=self.surrogate, alpha=self.alpha))
                layers.append(MergeTemporalDim(self.T))
                in_channels = cfg[i]

        layers.append(nn.Flatten())
        return layers
    
    def _make_classifier(self, in_channels):
        """
        Construct feature classifier.
        """
        layers = []
        if self.light_classifier:
            layers.append(nn.Linear(in_channels * 1 * 1, self.num_classes))
            layers.append(SplitTemporalDim(self.T))
            return layers
        
        layers.append(nn.Linear(in_channels * 1 * 1, 4096))
        layers.append(nn.BatchNorm1d(4096))
        layers.append(SplitTemporalDim(self.T))
        layers.append(LIF(surrogate_function=self.surrogate, alpha=self.alpha))
        layers.append(nn.Dropout(self.dropout))
        layers.append(MergeTemporalDim(self.T))

        layers.append(nn.Linear(4096, 4096))
        layers.append(nn.BatchNorm1d(4096))
        layers.append(SplitTemporalDim(self.T))
        layers.append(LIF(surrogate_function=self.surrogate, alpha=self.alpha))
        layers.append(nn.Dropout(self.dropout))
        layers.append(MergeTemporalDim(self.T))
        layers.append(nn.Linear(4096, self.num_classes))
        layers.append(SplitTemporalDim(self.T))
        return layers
    
    def forward(self, input):
        if self.has_temporal_dim:
            input = input.transpose(0, 1)
        output = self.layers(input)
        return output.mean(dim=0)