
import utils
from engine import train_one_epoch
from dataset import MultiShapeDataset
import torch
import torch.utils.data
import os
import time

os.makedirs("results", exist_ok=True)
writer = utils.log_writer("results", "maskrcnn")
num_classes = 4 # 0 for backgroud 
 
model = utils.get_instance_segmentation_model(num_classes).double()

model.load_state_dict(torch.load(r'your path to .pth',map_location='cpu'))

dataset = MultiShapeDataset(10)

torch.manual_seed(233)

data_loader = torch.utils.data.DataLoader(
    dataset, batch_size=2, num_workers=0, shuffle=True,
    collate_fn=utils.collate_fn)

params = [p for p in model.parameters() if p.requires_grad]
optimizer = torch.optim.SGD(params, lr=0.001,
                            momentum=0.9, weight_decay=0.0005)

lr_scheduler = torch.optim.lr_scheduler.StepLR(optimizer,
                                               step_size=3,
                                               gamma=0.1)

num_epochs = 3
device = torch.device('cpu')

count = 0
for epoch in range(num_epochs):
    t0 = time.time()
    count = train_one_epoch(model, optimizer, data_loader, device, count, writer)
    torch.save(model.state_dict(), "results/maskrcnn_"+str(epoch)+".pth") 
    lr_scheduler.step()

    print(f"Epoch {epoch} finished, time: {int(time.time()-t0) / 60.0} min.")