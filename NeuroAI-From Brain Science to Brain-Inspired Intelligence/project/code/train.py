import os
import random
import argparse
import time

import torch
import torch.nn as nn
from torch.utils.data import DataLoader
from torch.optim import AdamW, SGD
from torch.optim.lr_scheduler import CosineAnnealingLR
from torchinfo import summary
import numpy as np

from datasets.dataset import load_cifar10, load_cifar10dvs
from models.factory import build_model

from utils.config import config_parser
from utils.metric import AverageMeter, evaluate

import wandb
import tqdm
from spikingjelly.activation_based.functional import reset_net


def set_random_state(seed: int):
    random.seed(seed)
    os.environ['PYTHONHASHSEED'] = str(seed)
    os.environ['CUBLAS_WORKSPACE_CONFIG'] = ':4096:8'
    np.random.seed(seed)
    torch.manual_seed(seed)
    torch.cuda.manual_seed(seed)
    torch.cuda.manual_seed_all(seed)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False

def save_checkpoint(state, checkpoint_dir, filename):
    if not os.path.exists(checkpoint_dir):
        os.makedirs(checkpoint_dir)
    fpath = os.path.join(checkpoint_dir, filename)
    torch.save(state, fpath)


if __name__ == '__main__':
    # get command line args
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', dest='config', 
                        help='The path of config file', required=True)
    parser.add_argument('--seed', type=int, default=2025, 
                        help='Random seed (default: 2025)')
    parser.add_argument('--log', type=int, default=1, choices=[0, 1],
                        help='Log mode: 0 for off, 1 for loss&acc ' \
                        'curve and checkpoints')
    try:
        args = parser.parse_args()
    except:
        parser.print_help()
        exit(0)
    if args.config is None:
        raise Exception('Unrecognized config file: {args.config}.')
    else:
        config_path = args.config
    config = config_parser(config_path) 
    print(f'Config: {config}')

    # log config info to wandb
    if args.log:
        exp_name = f"{time.strftime('%Y%m%d-%H%M%S')}-stage1-{config['dataset']}-{config['arch']}{config['num_layers']}-{config['surrogate']}-{config['alpha']}"
        save_path = os.path.join(config['output_dir'], exp_name)
        if not os.path.exists(save_path):
            os.makedirs(save_path, exist_ok=True)
        wandb.init(project='NeuroAI-2025-Fall-Project',
                   config=config,
                   name=exp_name,
                   dir=save_path
                   )

    # set random state
    set_random_state(args.seed) 

    # load dataset
    dataset_dict = {'CIFAR10': load_cifar10,
                    'CIFAR10DVS': load_cifar10dvs
                    }

    if 'dvs' in config['dataset'].lower():
        train_set, val_set = dataset_dict[config['dataset']]\
                            (config['data_path'], config['T'])
    else:
        train_set, val_set = dataset_dict[config['dataset']](config['data_path'])

    train_loader = DataLoader(train_set,
                              shuffle=True,
                              batch_size=config['batch_size'],
                              num_workers=config['num_workers'],
                              pin_memory=True)
    val_loader = DataLoader(val_set,
                            batch_size=config['batch_size'],
                            shuffle=False,
                            num_workers=config['num_workers'],
                            pin_memory=True)
    
    # set device
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    print(f'Using device: {device}')

    # build model
    # TODO
    model = build_model(config).to(device)

    # summarize arch
    data_batch, _ = next(iter(train_loader))
    summary(model, 
            input_data=data_batch.to(device),
            device='cuda' if device.type == 'cuda' else 'cpu')
    reset_net(model)
    print(f"Architechture: {config['arch']}{config['num_layers']}")

    # set optimizer, scheduler, and loss function
    optimizer = config['optimizer'].lower()
    if optimizer == 'sgd':
        optimizer = SGD(model.parameters(), lr=config['learning_rate'], 
                        momentum=config['momentum'], 
                        weight_decay=config['weight_decay'])
    elif optimizer == 'adam':
        optimizer = AdamW(model.parameters(), lr=config['learning_rate'], 
                          weight_decay=config['weight_decay'])
    else:
        raise NotImplementedError(optimizer)
    
    scheduler = config['scheduler'].lower()
    if scheduler == 'cosalr':
        scheduler = CosineAnnealingLR(optimizer, 
                                    T_max=config['epochs'] * len(train_loader))
    else:
        raise NotImplementedError(scheduler)
    
    criterion = config['loss'].lower()
    if criterion == 'ce':
        criterion = nn.CrossEntropyLoss().to(device)
    else:
        raise NotImplementedError(criterion)
    
    # save initial checkpoint
    if args.log:
        save_checkpoint({
            'epoch': 0,
            'best_val_acc': 0.,
            'state_dict': model.state_dict(),
            'optimizer': optimizer.state_dict(),
            'scheduler': scheduler.state_dict(),
            'config': config,                    
            'seed': args.seed,             
        }, checkpoint_dir=save_path, filename='init_ckpt.pth')
    
    # start training
    best_val_acc = 0.
    iteration = 0
    for epoch in range(config['epochs']):
        model.train()
        train_loss = AverageMeter()
        top1 = AverageMeter()
        top5 = AverageMeter()
        tqdm_train_loop = tqdm.tqdm(train_loader)

        for idx, (inputs, labels) in enumerate(tqdm_train_loop):
            inputs, labels = inputs.to(device), labels.to(device)
            outputs = model(inputs)
            optimizer.zero_grad()
            batch_loss = criterion(outputs, labels)
            batch_loss.backward()
            optimizer.step()
            reset_net(model)

            train_loss.update(batch_loss.item(), labels.numel())
            acc1, acc5 = evaluate(outputs.detach(), labels, topk=(1, 5))
            top1.update(acc1.item(), labels.numel())
            top5.update(acc5.item(), labels.numel())

            tqdm_train_loop.set_postfix(lr=scheduler.get_last_lr()[0], 
                                        loss=train_loss.avg, 
                                        acc1=top1.avg, acc5=top5.avg)
            
            if args.log and iteration % config['log_interval'] == 0:
                wandb.log({
                    "train/loss": train_loss.avg,
                    "train/acc1": top1.avg,
                    "train/acc5": top5.avg,
                    "train/lr": optimizer.param_groups[0]['lr'],
                }, step=iteration)
            
            iteration += 1
            scheduler.step()
        
        print('Epoch%d:'%(epoch + 1))
        print('     Train Loss {loss.avg:.3f}'.format(loss=train_loss))
        print('     Train Acc@1 {top1.avg:.3f}'.format(top1=top1))
        print('     Train Acc@5 {top5.avg:.3f}'.format(top5=top5))

        model.eval()
        with torch.no_grad():
            val_loss = AverageMeter()
            top1 = AverageMeter()
            top5 = AverageMeter()
            for inputs, labels in tqdm.tqdm(val_loader):
                inputs, labels = inputs.to(device), labels.to(device)
                outputs = model(inputs)
                reset_net(model)
                
                batch_loss = criterion(outputs, labels)
                val_loss.update(batch_loss.item(), labels.numel())
                acc1, acc5 = evaluate(outputs.detach(), labels, topk=(1, 5))
                top1.update(acc1.item(), labels.numel())
                top5.update(acc5.item(), labels.numel())

            if args.log:
                wandb.log({
                    "Epoch": epoch,
                    "val/loss": val_loss.avg,
                    "val/acc1": top1.avg,
                    "val/acc5": top5.avg,
                }, step=iteration)
            print('     Val Loss {loss.avg:.3f}'.format(loss=val_loss))
            print('     Val Acc@1 {top1.avg:.3f}'.format(top1=top1))
            print('     Val Acc@5 {top5.avg:.3f}'.format(top5=top5))
            
            if top1.avg > best_val_acc:
                best_val_acc = top1.avg
                if args.log:
                    checkpoint = {
                        'epoch': epoch,
                        'best_val_acc': best_val_acc,
                        'state_dict': model.state_dict(),
                        'optimizer': optimizer.state_dict(),
                        'scheduler': scheduler.state_dict(),
                        'config': config,                    
                        'seed': args.seed,                   
                    }
                    save_checkpoint(checkpoint, checkpoint_dir=save_path, filename='best_ckpt.pth')

    print(f'Train finished')
    print(f'Best validation accuracy: {best_val_acc:.3f}')
    print(f'Args: {args}')