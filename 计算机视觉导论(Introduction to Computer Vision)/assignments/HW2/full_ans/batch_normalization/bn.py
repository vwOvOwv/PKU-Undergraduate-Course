import numpy as np
import cv2
import os


# eps may help you to deal with numerical problem
eps = 1e-5
def bn_forward_test(x, gamma, beta, mean, var):

    #----------------TODO------------------
    # Implement forward 
    #----------------TODO------------------

    x_hat = (x - mean) / (np.sqrt(var) + eps)
    out = gamma * x_hat + beta

    return out

def bn_forward_train(x, gamma, beta):

    #----------------TODO------------------
    # Implement forward 
    #----------------TODO------------------

    mean = np.mean(x, axis=0)
    var = np.var(x, axis=0)
    x_hat = (x - mean) / (np.sqrt(var) + eps)
    out = gamma * x_hat + beta

    # save intermidiate variables for computing the gradient when backward
    cache = (gamma, x, mean, var, x_hat)
    return out, cache
    
def bn_backward(dout, cache):

    #----------------TODO------------------
    # Implement backward 
    #----------------TODO------------------

    dx = dout * cache[0] / np.sqrt(cache[3])
    dgamma = (dout * cache[4]).sum(axis=0)
    dbeta = dout.sum(axis=0)

    return dx, dgamma, dbeta

# This function may help you to check your code
def print_info(x):
    print('mean:', np.mean(x,axis=0))
    print('var:',np.var(x,axis=0))
    print('------------------')
    return 

if __name__ == "__main__":
    HW_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    # input data
    train_data = np.zeros((9,784)) 
    for i in range(9):
        train_data[i,:] = cv2.imread(os.path.join(HW_dir, "mnist_subset", f"{i}.png"), cv2.IMREAD_GRAYSCALE).reshape(-1)/255.
    gt_y = np.zeros((9,1)) 
    gt_y[0] =1  

    val_data = np.zeros((1,784)) 
    val_data[0,:] = cv2.imread(os.path.join(HW_dir, "mnist_subset", "9.png"), cv2.IMREAD_GRAYSCALE).reshape(-1)/255.
    val_gt = np.zeros((1,1)) 

    np.random.seed(14)

    # Intialize MLP  (784 -> 16 -> 1)
    weight1 = np.random.randn(784,16)
    weight2 = np.random.randn(16,1)

    # Initialize gamma and beta
    gamma = np.random.randn(16)
    beta = np.random.randn(16)

    lr = 1e-1
    loss_list=[]

    # ---------------- TODO -------------------
    # compute mean and var for testing
    # add codes anywhere as you need
    # ---------------- TODO -------------------
    running_mean = 0.
    running_var = 0.

    # training 
    for i in range(50):
        # Forward
        z1 = train_data.dot(weight1)
        z1_bn, cache = bn_forward_train(z1, gamma, beta)
        momentum = 0.9
        running_mean = momentum * running_mean + (1 - momentum) * cache[2]
        running_var = momentum * running_var + (1 - momentum) * cache[3]
        a1 = 1 / (1 + np.exp(-z1_bn))  # sigmoid activation function
        z2 = a1.dot(weight2)
        pred_y = 1 / (1 + np.exp(-z2))  # sigmoid activation function

        # compute loss 
        loss = -(gt_y * np.log(pred_y) + (1 - gt_y) * np.log(1 - pred_y)).sum()
        print("iteration: %d, loss: %f" % (i+1 ,loss))
        loss_list.append(loss)

        # Backward : compute the gradient of paratmerters of layer1 (grad_layer_1) and layer2 (grad_layer_2)
        grad_pred_y = -(gt_y / pred_y) + (1 - gt_y) / (1 - pred_y)
        grad_z2 = grad_pred_y * pred_y * (1 - pred_y)
        grad_weight2 = a1.T.dot(grad_z2)
        grad_a1 = grad_z2.dot(weight2.T)
        grad_z1_bn  = grad_a1 * (1 - a1) * a1
        grad_z1, grad_gamma, grad_beta = bn_backward(grad_z1_bn, cache)
        grad_layer_1 = train_data.T.dot(grad_z1)

        # update parameters
        gamma -= lr * grad_gamma
        beta -= lr * grad_beta
        weight1 -= lr * grad_layer_1
        weight2 -= lr * grad_weight2
    
    # validate
    z1 = val_data.dot(weight1)
    z1_bn = bn_forward_test(z1, gamma, beta, running_mean, running_var)
    a1 = 1 / (1 + np.exp(-z1_bn))  # sigmoid activation function
    z2 = a1.dot(weight2)
    pred_y = 1 / (1 + np.exp(-z2))  # sigmoid activation function
    loss = -(val_gt * np.log(pred_y) + (1 - val_gt) * np.log(1 - pred_y)).sum()
    print("validation loss: %f" % (loss))
    loss_list.append(loss)

    os.makedirs(os.path.join(os.path.join(HW_dir), "results"), exist_ok=True)
    np.savetxt(os.path.join(os.path.join(HW_dir), "results", "bn_loss.txt"), loss_list)