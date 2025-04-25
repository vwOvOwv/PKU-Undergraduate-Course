import torch.nn as nn


class ConvNet(nn.Module):
    def __init__(self, num_class=10):
        super(ConvNet, self).__init__()
        self.num_class = num_class
        self.arch = [64, 64, 'M', 128, 128, 'M', 256, 256, 256, 'M', 512, 512, 512, 'M', 512, 512, 512, 'M']    # VGG16_bn
        self.conv_config = {'kernel_size': 3, 'stride': 1, 'padding': 1, 
                            'dilation': 1, 'groups': 1, 'padding_mode': 'zeros', 
                            'bias': True}
        
        # ----------TODO------------
        # define a network 
        # ----------TODO------------

        self.model = self._make_layers()

    def _make_layers(self):
        layers = []
        in_channels = 3 # for CIFAR10

        for i in range(len(self.arch)):
            if self.arch[i] == 'M':
                layers += [nn.MaxPool2d(2, 2, 0)]
            else:
                layers += [nn.Conv2d(in_channels, self.arch[i], **self.conv_config)]
                layers += [nn.BatchNorm2d(self.arch[i])]
                layers += [nn.ReLU()]
                in_channels = self.arch[i]
        
        layers += [nn.AdaptiveAvgPool2d((4, 4)), 
                   nn.Flatten()]

        layers += [nn.Linear(self.arch[-2] * 4 * 4, 4096)]
        layers += [nn.BatchNorm1d(num_features=4096)]   # add extra BN to stablize training
        layers += [nn.ReLU()]

        layers += [nn.Linear(4096, 4096)]
        layers += [nn.BatchNorm1d(num_features=4096)]   # add extra BN to stablize training
        layers += [nn.ReLU()]

        layers += [nn.Linear(4096, self.num_class)]
        return nn.Sequential(*layers)

    def forward(self, x):

        # ----------TODO------------
        # network forwarding 
        # ----------TODO------------

        x = self.model(x)
        return x


if __name__ == '__main__':
    import torch
    from torch.utils.tensorboard  import SummaryWriter
    from dataset import CIFAR10
    writer = SummaryWriter(log_dir='../experiments/network_structure')
    net = ConvNet()
    train_dataset = CIFAR10()
    train_loader = torch.utils.data.DataLoader(
        train_dataset, batch_size=2, shuffle=False, num_workers=2)
    # Write a CNN graph. 
    # Please save a figure/screenshot to '../results' for submission.
    for imgs, labels in train_loader:
        writer.add_graph(net, imgs)
        writer.close()
        break 
