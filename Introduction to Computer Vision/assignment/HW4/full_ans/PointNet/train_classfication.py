from __future__ import print_function
import os
os.environ["KMP_DUPLICATE_LIB_OK"]= "TRUE"
import random
import torch
import torch.nn.parallel
import torch.optim as optim
import torch.utils.data
from dataset import ShapeNetClassficationDataset
from model import PointNetCls256D, PointNetCls1024D
import torch.nn.functional as F
from tqdm import tqdm
from utils import log_writer, setting
from os.path import join as pjoin
import argparse

if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('--dim', '-d', required=True, type=int, help='feature dim, choose 256 or 1024')
    args = parser.parse_args()

    opt = setting()
    writer = log_writer(opt.expf, f"cls_{args.dim}D")

    blue = lambda x: '\033[94m' + x + '\033[0m'

    random.seed(opt.manualSeed)
    torch.manual_seed(opt.manualSeed)

    dataset = ShapeNetClassficationDataset(
        root=opt.dataset,
        npoints=opt.num_points)

    test_dataset = ShapeNetClassficationDataset(
        root=opt.dataset,
        split='test',
        npoints=opt.num_points,
        with_data_augmentation=False)

    dataloader = torch.utils.data.DataLoader(
        dataset,
        batch_size=opt.batchSize,
        shuffle=True,
        num_workers=int(opt.workers))

    testdataloader = torch.utils.data.DataLoader(
            test_dataset,
            batch_size=opt.batchSize,
            shuffle=True,
            num_workers=int(opt.workers))

    print(len(dataset), len(test_dataset))
    num_classes = len(dataset.classes)
    print('classes', num_classes)

    if args.dim == 256:
        classifier = PointNetCls256D(k=num_classes)
    elif args.dim == 1024:
        classifier = PointNetCls1024D(k=num_classes)
    else:
        raise NotImplementedError

    optimizer = optim.Adam(classifier.parameters(), lr=0.01, betas=(0.9, 0.999))
    scheduler = optim.lr_scheduler.StepLR(optimizer, step_size=1, gamma=0.8)
    # classifier.cuda()

    num_batch = len(dataset) / opt.batchSize

    count = 0 
    for epoch in range(opt.nepoch):
        for i, data in enumerate(dataloader, 0):
            points, target = data
            target = target[:, 0]
            optimizer.zero_grad()
            classifier = classifier.train()
            pred, _ = classifier(points)
            loss = F.nll_loss(pred, target)
            loss.backward()
            optimizer.step()
            pred_choice = pred.data.max(1)[1]
            correct = pred_choice.eq(target.data).cpu().float().mean()
            print('[%d: %d/%d] train loss: %f accuracy: %f' % (epoch, i, num_batch, loss.item(), correct.item()))
            writer.add_train_scalar("Loss", loss.item(), count )
            writer.add_train_scalar("Acc", correct.item(), count)

            if i % 10 == 0:
                j, data = next(enumerate(testdataloader, 0))
                points, target = data
                target = target[:, 0]
                classifier = classifier.eval()
                pred, _= classifier(points)
                loss = F.nll_loss(pred, target)
                pred_choice = pred.data.max(1)[1]
                correct = pred_choice.eq(target.data).cpu().float().mean()
                print('[%d: %d/%d] %s loss: %f accuracy: %f' % (epoch, i, num_batch, blue('test'), loss.item(), correct.item()))

                writer.add_test_scalar("Loss", loss.item(), count )
                writer.add_test_scalar("Acc", correct.item(), count)
            count +=1

        torch.save(classifier.state_dict(), pjoin(opt.expf, f'cls_{args.dim}D/model.pth'))
        scheduler.step()

    total_correct = 0
    total_testset = 0
    for i,data in tqdm(enumerate(testdataloader, 0)):
        points, target = data
        target = target[:, 0]
        classifier = classifier.eval()
        pred, _ = classifier(points)
        pred_choice = pred.data.max(1)[1]
        correct = pred_choice.eq(target.data).cpu().sum()
        total_correct += correct.item()
        total_testset += points.size()[0]

    print("final accuracy {}".format(total_correct / float(total_testset)))