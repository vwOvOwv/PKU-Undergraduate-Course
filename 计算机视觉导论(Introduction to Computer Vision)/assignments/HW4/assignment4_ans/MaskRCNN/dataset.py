import os
import numpy as np
import torch
import torch.utils.data
import random
import cv2
import math 
from utils import plot_save_dataset


class SingleShapeDataset(torch.utils.data.Dataset):
    '''
        SingleShapeDataset: only one shape in each image
    '''
    def __init__(self, size):

        self.w = 128
        self.h = 128
        self.size = size
        print("size",self.size)


    def _draw_shape(self, img, mask, shape_id):
        buffer = 20
        y = random.randint(buffer, self.h - buffer - 1)
        x = random.randint(buffer, self.w - buffer - 1)
        s = random.randint(buffer, self.h//4)
        color = tuple([random.randint(0, 255) for _ in range(3)])

        
        if shape_id == 1:
            cv2.rectangle(mask, (x-s, y-s), (x+s, y+s), 1, -1)
            cv2.rectangle(img, (x-s, y-s), (x+s, y+s), color, -1)

        elif shape_id == 2:
            cv2.circle(mask, (x, y), s, 1, -1)
            cv2.circle(img, (x, y), s, color, -1)

        elif shape_id == 3:
            points = np.array([[(x, y-s),
                            (x-s/math.sin(math.radians(60)), y+s),
                            (x+s/math.sin(math.radians(60)), y+s),
                            ]], dtype=np.int32)
            cv2.fillPoly(mask, points, 1)
            cv2.fillPoly(img, points, color)


    def __getitem__(self, idx):
        np.random.seed(idx)
        random.seed(idx)

        num_objs = 1
        masks = np.zeros((num_objs, self.h, self.w))
        img = np.zeros((self.h, self.w, 3))
        img[...,:] = np.asarray([random.randint(0, 255) for _ in range(3)])[None, None, :]


        obj_ids = np.zeros((num_objs)) 

        shape_code = random.randint(1,3)
        self._draw_shape( img, masks[0, :], shape_code)
        obj_ids[0] = shape_code


        boxes = np.zeros((num_objs,4))
        pos = np.where(masks[0])
        xmin = np.min(pos[1])
        xmax = np.max(pos[1])
        ymin = np.min(pos[0])
        ymax = np.max(pos[0])
        boxes[0,:] = np.asarray([xmin, ymin, xmax, ymax])

        boxes = torch.as_tensor(boxes, dtype=torch.float32)
        labels = torch.as_tensor(obj_ids, dtype=torch.int64)
        masks = torch.as_tensor(masks, dtype=torch.uint8)

        image_id = torch.tensor([idx])
        area = (boxes[:, 3] - boxes[:, 1]) * (boxes[:, 2] - boxes[:, 0])

        target = {}
        target["boxes"] = boxes
        target["labels"] = labels
        target["masks"] = masks
        target["image_id"] = image_id
        target["area"] = area
        img = torch.tensor(img)
        img = img.permute(2,0,1)


        return img, target


    def __len__(self):
        return self.size


def compute_iou(box, boxes, box_area, boxes_area):
    ## ----------------------------- TODO ----------------------- ##
    ## Compute the IoU between the specified box and other boxes  ##
    ## ---------------------------------------------------------- ##
    return iou


def nom_max_suppression(boxes, threshold):
    
    x1 = boxes[:,0]
    y1 = boxes[:,1]
    x2 = boxes[:,2]
    y2 = boxes[:,3]

    area = (y2 - y1) * (x2 - x1)
    ixs = np.arange(boxes.shape[0]-1, -1, -1)
    pick = []
    # Iterate while some indexes still remain in the indexes list
    # For the current index, calculate the IoU with all other indexes,
    # and remove indexes with IoU greater than the threshold
    while len(ixs) > 0:
        i = ixs[0]
        if area[i] == 0:
            continue
        pick.append(i)
        iou = compute_iou(boxes[i,:], boxes[ixs[1:],:], area[i], area[ixs[1:]])
        ## ----------------------------- TODO ----------------------- ##
        ## Remove the indexes with IoU greater than the threshold     ##
        ixs = 
        ## ---------------------------------------------------------- ##
    
    return np.array(pick, dtype=np.int32)


class MultiShapeDataset(torch.utils.data.Dataset):
    def __init__(self, size):

        self.w = 128
        self.h = 128
        self.size = size


    def _draw_one_shape_on_mask(self,  mask, shape_id):
        buffer = 20
        y = random.randint(buffer, self.h - buffer - 1)
        x = random.randint(buffer, self.w - buffer - 1)
        s = random.randint(buffer, self.h//4)

        if shape_id == 1:
            cv2.rectangle(mask, (x-s, y-s), (x+s, y+s), 1, -1)

        elif shape_id == 2:
            cv2.circle(mask, (x, y), s, 1, -1)

        elif shape_id == 3:
            points = np.array([[(x, y-s),
                            (x-s/math.sin(math.radians(60)), y+s),
                            (x+s/math.sin(math.radians(60)), y+s),
                            ]], dtype=np.int32)
            cv2.fillPoly(mask, points, 1)
        
        return np.asarray([y, x, s, shape_id])


    def _draw_one_shape_on_img(self, img, param):

        color = tuple([random.randint(0, 255) for _ in range(3)])
        y = param[0]
        x = param[1]
        s = param[2]
        shape_id = param[3]

        if shape_id == 1:
            # print(str(x-s), str(y-s),str(x+s), str(y+s))
            cv2.rectangle(img, (x-s, y-s), (x+s, y+s), color, -1)

        elif shape_id == 2:
            cv2.circle(img, (x, y), s, color, -1)

        elif shape_id == 3:
            points = np.array([[(x, y-s),
                            (x-s/math.sin(math.radians(60)), y+s),
                            (x+s/math.sin(math.radians(60)), y+s),
                            ]], dtype=np.int32)
            cv2.fillPoly(img, points, color)


    def __getitem__(self, idx):
        np.random.seed(idx)
        random.seed(idx)
        num_objs = random.randint(2,4) # number of shapes in single image

        masks = np.zeros((num_objs, self.h, self.w))
        img = np.zeros((self.h, self.w, 3))

        ## ------------------ TODO ------------------- ##
        ## randomize the background color of the image ##
        img = 

        ## ------------------------------------------- ##
        

        ## ------------------------ TODO --------------------- ##
        ## randomize the type, position and size of the shapes ##
        obj_param = np.zeros((num_objs, 4)) 
        obj_ids = np.zeros((num_objs)) 



        ## --------------------------------------------------- ##
        

        boxes = np.zeros((num_objs,4))
        for i in range(num_objs):
            pos = np.where(masks[i])
            xmin = np.min(pos[1])
            xmax = np.max(pos[1])
            ymin = np.min(pos[0])
            ymax = np.max(pos[0])
            boxes[i,:] = np.asarray([xmin, ymin, xmax, ymax])

        
        # We do not want large occlusions between shapes. 
        # So we use non-max suppression (NMS) to remove the shapes with large overlap.
        indx = nom_max_suppression(boxes, 0.2)

        masks = masks[indx,...]
        obj_ids = obj_ids[indx]
        boxes = boxes[indx,...]
        obj_param = obj_param[indx,...].astype(int)


        ## ------------------------ TODO --------------------- ##
        ## update the masks to handle occlusions               ##




        ## --------------------------------------------------- ##


        for i in range(obj_param.shape[0]-1, -1, -1):
            self._draw_one_shape_on_img(img, obj_param[i,:])


        boxes = torch.as_tensor(boxes, dtype=torch.float32)
        labels = torch.as_tensor(obj_ids, dtype=torch.int64)
        masks = torch.as_tensor(masks, dtype=torch.uint8)

        image_id = torch.tensor([idx])
        area = (boxes[:, 3] - boxes[:, 1]) * (boxes[:, 2] - boxes[:, 0])

        target = {}
        target["boxes"] = boxes
        target["labels"] = labels
        target["masks"] = masks
        target["image_id"] = image_id
        target["area"] = area
        img = torch.tensor(img)
        img = img.permute(2,0,1)

        return img, target


    def __len__(self):
        return self.size


if __name__ == '__main__':
    dataset = MultiShapeDataset(10)
    os.makedirs("results", exist_ok=True)
    path = "results/" 

    for i in range(4):
        imgs, labels = dataset[i]
        plot_save_dataset(path+str(i)+"_data.png", imgs, labels)