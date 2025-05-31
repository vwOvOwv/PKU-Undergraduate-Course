from __future__ import print_function
import torch
import torch.nn as nn
import torch.nn.parallel
import torch.utils.data
import torch.nn.functional as F
from utils import setting


class PointNetMLPBlock(nn.Module):
    def __init__(self, in_features, out_features):
        super(PointNetMLPBlock, self).__init__()
        self.linear = nn.Linear(in_features, out_features)
        self.bn = nn.BatchNorm1d(out_features)
        self.relu = nn.ReLU()

    def forward(self, x):
        x_linear = self.linear(x)
        x_permuted = x_linear.permute(0, 2, 1)
        x_bn = self.bn(x_permuted)
        x_restored = x_bn.permute(0, 2, 1)
        output = self.relu(x_restored)
        return output

class PointNetfeat(nn.Module):
    '''
        The feature extractor in PointNet, corresponding to the left MLP in the pipeline figure.
        Args:
        d: the dimension of the global feature, default is 1024.
        segmentation: whether to perform segmentation, default is True.
    '''
    def __init__(self, segmentation=True, d=1024):
        super(PointNetfeat, self).__init__()
        ## ------------------- TODO ------------------- ##
        ## Define the layers in the feature extractor. ##
        ## ------------------------------------------- ##
        self.segmentation = segmentation
        self.layer_1 = PointNetMLPBlock(in_features=3, out_features=64)
        self.layer_2 = PointNetMLPBlock(in_features=64, out_features=128)
        self.layer_3 = PointNetMLPBlock(in_features=128, out_features=d)

    def forward(self, x):
        '''
            x: (B, N, 3)
            If segmentation == True
                return the concatenated global feature and local feature. # (B, N, d+64)
            If segmentation == False
                return the global feature, and the per point feature for cruciality visualization in question b). # (B, d), (B, N, d)
            Here, B is the batch size, N is the number of points, d is the dimension of the global feature.
        '''
        ## ------------------- TODO ------------------- ##
        ## Implement the forward pass.                 ##
        ## ------------------------------------------- ##
        feature_1 = self.layer_1(x)    # (B, N, 64)
        feature_2 = self.layer_2(feature_1)
        per_point_feature = self.layer_3(feature_2)    # (B, N, d)
        
        global_feature, _ = torch.max(per_point_feature, dim=1)  # max pooling, (B, d)
        repeat_global_feature = global_feature.unsqueeze(dim=1).repeat(
            1, setting.num_points, 1
        )    # (B, N, d)
        if self.segmentation:
            concat_feature = torch.concatenate((repeat_global_feature, feature_1), dim=2)    # (B, N, d+64)
            return concat_feature
        else:
            return global_feature, per_point_feature

class PointNetCls1024D(nn.Module):
    '''
        The classifier in PointNet, corresponding to the middle right MLP in the pipeline figure.
        Args:
        k: the number of classes, default is 2.
    '''
    def __init__(self, k=2):
        super(PointNetCls1024D, self).__init__()
        ## ------------------- TODO ------------------- ##
        ## Define the layers in the classifier.        ##
        ## ------------------------------------------- ##
        self.feature_extractor = PointNetfeat(segmentation=False, d=1024)
        self.classifier = nn.Sequential(
            nn.Linear(in_features=1024, out_features=512),
            nn.BatchNorm1d(num_features=512),
            nn.ReLU(),
            nn.Linear(in_features=512, out_features=256),
            nn.BatchNorm1d(num_features=256),
            nn.ReLU(),
            nn.Linear(in_features=256, out_features=k),
            nn.LogSoftmax(dim=1)
        )

    def forward(self, x):
        '''
            return the log softmax of the classification result and the per point feature for cruciality visualization in question b). # (B, k), (B, N, d=1024)
        '''
        ## ------------------- TODO ------------------- ##
        ## Implement the forward pass.                 ##
        ## ------------------------------------------- ##
        global_feature, _ = self.feature_extractor(x)   # (B, 1024)
        output = self.classifier(global_feature)
        return output, _

class PointNetCls256D(nn.Module):
    '''
        The classifier in PointNet, corresponding to the upper right MLP in the pipeline figure.
        Args:
        k: the number of classes, default is 2.
    '''
    def __init__(self, k=2):
        super(PointNetCls256D, self).__init__()
        ## ------------------- TODO ------------------- ##
        ## Define the layers in the classifier.        ##
        ## ------------------------------------------- ##
        self.feature_extractor = PointNetfeat(segmentation=False, d=256)
        self.classifier = nn.Sequential(
            nn.Linear(in_features=256, out_features=128),
            nn.BatchNorm1d(num_features=128),
            nn.ReLU(),
            nn.Linear(in_features=128, out_features=k),
            nn.LogSoftmax(dim=1)
        )

    def forward(self, x):
        '''
            return the log softmax of the classification result and the per point feature for cruciality visualization in question b). # (B, k), (B, N, d=256)
        '''
        ## ------------------- TODO ------------------- ##
        ## Implement the forward pass.                 ##
        ## ------------------------------------------- ##
        global_feature, _ = self.feature_extractor(x)
        output = self.classifier(global_feature)
        return output, None

class PointNetSeg(nn.Module):
    '''
        The segmentation head in PointNet, corresponding to the lower right MLP in the pipeline figure.
        Args:
        k: the number of classes, default is 2.
    '''
    def __init__(self, k = 2):
        super(PointNetSeg, self).__init__()
        ## ------------------- TODO ------------------- ##
        ## Define the layers in the segmentation head. ##
        ## ------------------------------------------- ##
        self.feature_extractor = PointNetfeat(segmentation=True, d=1024)
        self.classifier = nn.Sequential(
            PointNetMLPBlock(in_features=64+1024, out_features=512),
            PointNetMLPBlock(in_features=512, out_features=256),
            PointNetMLPBlock(in_features=256, out_features=128),
            nn.Linear(in_features=128, out_features=k),
            nn.LogSoftmax(dim=2)
        )

    def forward(self, x):
        '''
            Input:
                x: the input point cloud. # (B, N, 3)
            Output:
                the log softmax of the segmentation result. # (B, N, k)
        '''
        ## ------------------- TODO ------------------- ##
        ## Implement the forward pass.                 ##
        ## ------------------------------------------- ##
        concat_feature = self.feature_extractor(x)
        output = self.classifier(concat_feature)
        return output
