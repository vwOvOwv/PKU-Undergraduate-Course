import utils
from dataset import SingleShapeDataset
from utils import plot_save_output
import torch
import torch.utils.data


dataset_test = SingleShapeDataset(10)
data_loader_test = torch.utils.data.DataLoader(
    dataset_test, batch_size=1, shuffle=False, num_workers=0,
    collate_fn=utils.collate_fn)
 

num_classes = 4
 
# get the model using the helper function
model = utils.get_instance_segmentation_model(num_classes).double()

model.load_state_dict(torch.load('/Users/peiyuliu/Desktop/PKU-Undergraduate-Course-Public/计算机视觉导论(Introduction to Computer Vision)/assignments/HW4/assignment4_ans/MaskRCNN/results/maskrcnn_2.pth', map_location='cpu'))


model.eval()
path = "results/" 
for i in range(4):
    imgs, labels = dataset_test[i]
    output = model([imgs])
    plot_save_output(path+str(i)+"_result.png", imgs, output[0])