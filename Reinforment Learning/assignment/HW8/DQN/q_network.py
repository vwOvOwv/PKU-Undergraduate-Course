import torch
from torch import nn

class QNetwork(nn.Module):

    def __init__(self, in_channels, output_dim, lr):
        super(QNetwork, self).__init__()
        self.seq = nn.Sequential(
            # (in_channels, 80, 80)
            nn.Conv2d(in_channels, 32, kernel_size=8, stride=4),
            nn.ReLU(),
            # (32, 19, 19)
            nn.Conv2d(32, 64, kernel_size=4, stride=2),
            nn.ReLU(),
            # (64, 8, 8)
            nn.Conv2d(64, 64, kernel_size=3, stride=1),
            nn.ReLU(),
            # (64, 6, 6)
            nn.Flatten(),
            nn.Linear(64 * 6 * 6, 512),
            nn.ReLU(),
            nn.Linear(512, output_dim)
        )
        for m in self.modules():
            if isinstance(m, nn.Conv2d):
                nn.init.kaiming_normal_(m.weight)
            elif isinstance(m, nn.Linear):
                nn.init.kaiming_normal_(m.weight)
        self.optimizer = torch.optim.Adam(self.parameters(), lr=lr)
    
    def inference(self, obs):
        if obs.dim() == 3:
            obs = obs.unsqueeze(0)
        q_value = self.seq(obs)
        return q_value
    
    def update(self, loss):
        self.optimizer.zero_grad()
        loss.backward()
        torch.nn.utils.clip_grad_norm_(self.parameters(), max_norm=10.0)
        self.optimizer.step()