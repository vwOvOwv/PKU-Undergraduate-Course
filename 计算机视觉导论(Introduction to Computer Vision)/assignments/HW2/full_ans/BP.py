import numpy as np
import cv2
import os


def read_img(path):
    return cv2.imread(path, cv2.IMREAD_GRAYSCALE)


if __name__ == "__main__":

    # Input
    input = np.zeros((10, 784)) 
    for i in range(10):
        input[i,:] = read_img("mnist_subset/"+str(i)+".png").reshape(-1) / 255.
    gt_y = np.zeros((10, 1)) 
    gt_y[0] =1

    np.random.seed(14)

    # MLP Intialization  (784 -> 16 -> 1)
    weight_1 = np.random.randn(784, 16)
    weight_2 = np.random.randn(16, 1)
    lr = 1e-1
    loss_list=[]

    for i in range(50):
        # Forward
        z_1 = input.dot(weight_1)  # (10, 16)
        a_1 = 1 / (1 + np.exp(-z_1))  # Sigmoid activation function (10, 16)
        z_2 = a_1.dot(weight_2)    # (10, 1)
        a_2 = 1 / (1 + np.exp(-z_2))  # Sigmoid activation function (10, 1)
        loss = -(gt_y * np.log(a_2) + (1 - gt_y) * np.log(1 - a_2)).sum() #cross-entropy loss
        print("iteration: %d, loss: %f" % (i + 1 ,loss))
        loss_list.append(loss)

        # Backward: compute the gradient of paratmerters of layer1 (grad_layer_1) and layer2 (grad_layer_2)
        grad_a_2 = - gt_y / a_2 + (1 - gt_y) / (1 - a_2) # (10, 1)
        a_2_z_2 = 1 / ((1 + np.exp(-z_2)) * (1 + np.exp(z_2)))  # (10, 1)
        grad_z_2 = grad_a_2 * a_2_z_2   # (10, 1)
        grad_weight_2 = a_1.T @ grad_z_2    # (16, 10) x (10, 1)
        
        grad_a_1 = grad_z_2 @ weight_2.T # (10, 1) x (1, 16)
        a_1_z_1 = 1 / ((1 + np.exp(-z_1)) * (1 + np.exp(z_1)))  # (10, 16)
        grad_z_1 = grad_a_1 * a_1_z_1 # (10, 16)
        grad_weight_1 = input.T @ grad_z_1  # (784, 10) x (10, 16)

        # print(loss_weight_2.shape)
        weight_1 -= lr * grad_weight_1
        weight_2 -= lr * grad_weight_2

    os.makedirs("results", exist_ok=True)
    np.savetxt("results/BP.txt", loss_list)