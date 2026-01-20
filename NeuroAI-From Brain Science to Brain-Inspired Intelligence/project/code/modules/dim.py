import torch.nn as nn
import torch

class AddTemporalDim(nn.Module):
    """
    Add a temporal dimension to the input tensor.
    """
    def __init__(self, T):
        super(AddTemporalDim, self).__init__()
        self.T = T

    def forward(self, x: torch.Tensor):
        return x.unsqueeze(0).repeat(self.T, *torch.ones(x.ndim, dtype=torch.int64))
    

class MergeTemporalDim(nn.Module):
    """
    Merge temporal dim into batch dim.
    """
    def __init__(self, T):
        super(MergeTemporalDim, self).__init__()
        self.T = T

    def forward(self, x: torch.Tensor):
        return x.flatten(0, 1).contiguous()
    

class SplitTemporalDim(nn.Module):
    """
    Split temporal dim and batch dim.
    """
    def __init__(self, T):
        super(SplitTemporalDim, self).__init__()
        self.T = T

    def forward(self, x: torch.Tensor):
        y_shape = [self.T, int(x.shape[0] / self.T)]
        y_shape.extend(x.shape[1:])
        return x.view(y_shape)