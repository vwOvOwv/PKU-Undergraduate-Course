# Installation

The Installation is the almost the same as HM2. So you can **directly use the HM2 environment and pip install 3 more pacages as listed below** or create new environment by following the below steps:

- We recommand using [Anaconda](https://www.anaconda.com/) to manage your python environments. Use the following command to create a new environment. 
```bash
conda create -n hw4 python=3.7 # use python=3.8 on Mac
conda activate hw4
```

- We recommand using [Tsinghua Mirror](https://mirrors.tuna.tsinghua.edu.cn/) to install dependent packages.

```bash
# pip
python -m pip install --upgrade pip
pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple

# conda
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/pytorch
conda config --add channels  https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main
conda config --add channels  https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free
conda config --set show_channel_urls yes
```

- Now you can install [pytorch](https://pytorch.org/get-started/previous-versions/) and other dependencies as below. Choose the version that fits your machine. The specific version of pytorch should make no functional difference for this assignment, since we only use some basic functions. You can also install the GPU version if you can access a GPU.
```bash
conda install pytorch==1.7.1 torchvision==0.8.2 torchaudio==0.7.2 cpuonly # remember to remove "-c pytorh"!

# tips: always try "pip install xxx" first before "conda install xxx"
pip install opencv-python
pip install pillow
pip install tensorboardx
pip install matplotlib # new for HW4
pip install imageio # new for HW4
pip install h5py # new for HW4
```
You can also install the GPU version if you can access a GPU.

# ShapePartNet for PointNet

## Dataset
- Download: you can either download the ShapePartNet dataset from source (require VPN): 
```bash
wget https://shapenet.cs.stanford.edu/ericyi/shapenetcore_partanno_segmentation_benchmark_v0.zip --no-check-certificate
```
, or from a [mirror version](https://disk.pku.edu.cn/link/AA6B205C10C4F34D30A69869B118BE9762) in our PKU disk.

Then unzip it:
```bash
unzip shapenetcore_partanno_segmentation_benchmark_v0.zip
```

## Data Configuration
Open `PointNet/utils.py`, and modify the dataset path:
```
dataset = "YOUR_PATH/shapenetcore_partanno_segmentation_benchmark_v0"
```


## Visualization

- Train network and visualize the curves
```bash
cd PointNet
python train_classification.py -d 256
cd ../exps
tensorboard --logdir .
```


# Mask RCNN
The skeleton code of Mask RCNN is based on [torchvision](https://pytorch.org/vision/stable/index.html). And you can find more detials in [https://pytorch.org/tutorials/intermediate/torchvision_tutorial.html](https://pytorch.org/tutorials/intermediate/torchvision_tutorial.html).

You can obtain the pre-trained weight from [here](https://disk.pku.edu.cn/link/AA751DE237ED8448C1A9EFD11B4438D303).


# RNN
- Download and unzip the dataset from [here](https://disk.pku.edu.cn/link/AA3D20C64ECC9D4AF8AC69428DCAC5638C).
- Then unzip it and set the variable `BASE_DIR='your_path_to_coco_captioning_folder'` in `RNN/utils/coco_utils.py`.


# Submission
- compress your code and results using our provided script \textit{pack.py} and submit to \href{course.pku.edu.cn}{course.pku.edu.cn}.

# Appendix and Acknowledgement
We list some libraries that may help you solve this assignment.

- [TensorboardX](https://pytorch.org/docs/stable/tensorboard.html)
- [OpenCV-Python](https://docs.opencv.org/4.x/d6/d00/tutorial_py_root.html)
- [Pillow (PIL)](https://pillow.readthedocs.io/en/stable/)
- [Torchvision.transforms](https://pytorch.org/vision/0.9/transforms.html)

Our code is inpired by [PointNet-Pytorch](https://github.com/fxia22/pointnet.pytorch), [detection-torchvision](https://pytorch.org/tutorials/intermediate/torchvision_tutorial.html) and cs231n.