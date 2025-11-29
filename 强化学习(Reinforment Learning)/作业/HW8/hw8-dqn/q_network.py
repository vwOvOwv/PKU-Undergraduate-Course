import torch
from torch import nn

class QNetwork(nn.Module):

    def __init__(self, input_dim, output_dim, lr):
        super(QNetwork, self).__init__()
        self.seq = nn.Sequential(
            nn.Linear(input_dim, 64),
            nn.ReLU(),
            nn.Linear(64, output_dim)
        )
        for m in self.modules():
            if isinstance(m, nn.Linear):
                nn.init.kaiming_normal_(m.weight)
        self.optimizer = torch.optim.Adam(self.parameters(), lr = lr)
    
    def inference(self, obs):
        q_value = self.seq(obs)
        return q_value
    
    def train(self, loss):
        self.optimizer.zero_grad()
        loss.backward()
        self.optimizer.step()