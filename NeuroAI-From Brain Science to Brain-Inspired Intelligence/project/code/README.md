# Activation-based Training of Spiking Neural Networks
Codes for the team project of NeuroAI at PKU, 2025 Fall.

This repository contains the implementation of activation-based training 
framework for vanilla SNNs, with surrogate gradients and BPTT.

VGG and ResNet are supported and can be trained on 
CIFAR-10/CIFAR10-DVS datasets.

## Environment Setup
```bash
conda create -n neuroai-proj python=3.10 -y
conda activate neuroai-proj
pip install -r requirements.txt
```

## Usage
```bash
python train.py [-h] --config CONFIG [--seed SEED] [--log {0,1}]
options:
  -h, --help       show this help message and exit
  --config CONFIG  The path of config file
  --seed SEED      Random seed (default: 2025)
  --log {0,1}      Log mode: 0 for off, 1 for loss&acc curve and checkpoints (default: 1)
```

Example:
```bash
python train.py --config configs/CIFAR10-ResNet18.yaml
```

You can easily explore more surrogate gradients and finetune hyperparameters 
by modifying the config files.