import numpy as np
from HM1_Convolve import Gaussian_filter, Sobel_filter_x, Sobel_filter_y, padding
from utils import read_img, write_img
from collections import deque

def compute_gradient_magnitude_direction(x_grad:np.ndarray, y_grad:np.ndarray):
    """
        The function you need to implement for Q2 a).
        Inputs:
            x_grad: array(float) 
            y_grad: array(float)
        Outputs:
            magnitude_grad: array(float)
            direction_grad: array(float) you may keep the angle of the gradient at each pixel
    """
    magnitude_grad = np.sqrt(x_grad**2 + y_grad**2)
    direction_grad = np.arctan2(y_grad, x_grad) * 180 / np.pi # [-180, 180]
    return magnitude_grad, direction_grad 



def non_maximal_suppressor(grad_mag:np.ndarray, grad_dir:np.ndarray):
    """
        The function you need to implement for Q2 b).
        Inputs:
            grad_mag: array(float) 
            grad_dir: array(float)
        Outputs:
            output: array(float)
    """   
    H_in, W_in = grad_mag.shape
    # make zero padding first to avoid edge cases when rolling
    padding_grad_mag = padding(grad_mag, 1, "zeroPadding")

    # calcultate local maximums for different cases
    # never forget that x axis corresponds to columns
    """
    -----------------------------> x
    """
    left_right_mask = (((grad_dir > -22.5) & (grad_dir < 22.5)) | \
                        ((grad_dir > 157.5) & (grad_dir <= 180)) | \
                        ((grad_dir >= -180) & (grad_dir < -157.5))).astype(int)
    left_right_pixels = grad_mag * left_right_mask
    filtered_left_right_pixels = left_right_pixels * \
        (left_right_pixels > np.roll(padding_grad_mag, 1, axis=1)[1: H_in + 1, 1: W_in + 1]).astype(int) * \
        (left_right_pixels > np.roll(padding_grad_mag, -1, axis=1)[1: H_in + 1, 1: W_in + 1]).astype(int)
    
    up_down_mask = (((grad_dir > 67.5) & (grad_dir < 112.5)) | \
                    ((grad_dir < -67.5) & (grad_dir > -112.5))).astype(int)
    up_down_pixels = grad_mag * up_down_mask
    filtered_up_down_pixels = up_down_pixels * \
        (up_down_pixels > np.roll(padding_grad_mag, 1, axis=0)[1: H_in + 1, 1: W_in + 1]).astype(int) * \
        (up_down_pixels > np.roll(padding_grad_mag, -1, axis=0)[1: H_in + 1, 1: W_in + 1]).astype(int)
    
    leftup_rightdown_mask = (((grad_dir > 22.5) & (grad_dir < 67.5)) | \
                    ((grad_dir < -112.5) & (grad_dir > -157.5))).astype(int)
    leftup_rightdown_pixels = grad_mag * leftup_rightdown_mask
    filtered_leftup_rightdown_pixels = leftup_rightdown_pixels * \
        (leftup_rightdown_pixels > np.roll(np.roll(padding_grad_mag, 1, axis=0), 1, axis=1)[1: H_in + 1, 1: W_in + 1]).astype(int) * \
        (leftup_rightdown_pixels > np.roll(np.roll(padding_grad_mag, -1, axis=0), -1, axis=1)[1: H_in + 1, 1: W_in + 1]).astype(int)
    
    rightup_leftdown_mask = (((grad_dir > 112.5) & (grad_dir < 157.5)) | \
                    ((grad_dir < -22.5) & (grad_dir > -67.5))).astype(int)
    rightup_leftdown_pixels = grad_mag * rightup_leftdown_mask
    filtered_rightup_leftdown_pixels = rightup_leftdown_pixels * \
        (rightup_leftdown_pixels > np.roll(np.roll(padding_grad_mag, 1, axis=0), -1, axis=1)[1: H_in + 1, 1: W_in + 1]).astype(int) * \
        (rightup_leftdown_pixels > np.roll(np.roll(padding_grad_mag, -1, axis=0), 1, axis=1)[1: H_in + 1, 1: W_in + 1]).astype(int)

    # since the whole img is divided into four parts and then filtered, we can
    # directly add the filtering results to get the NMS output.
    NMS_output = filtered_left_right_pixels + filtered_up_down_pixels + \
        filtered_leftup_rightdown_pixels + filtered_rightup_leftdown_pixels
    
    return NMS_output 
            
def hysteresis_thresholding(img:np.ndarray):
    """
        The function you need to implement for Q2 c).
        Inputs:
            img: array(float) 
        Outputs:
            output: array(float)
    """

    # you can adjust the parameters to fit your own implementation 
    low_ratio = 1.
    high_ratio = 2.
    mean = img[np.nonzero(img)].mean()  # calculate mean value of local maximums
    min_val = low_ratio * mean
    max_val = high_ratio * mean
    img *= (img >= min_val).astype(int) # remove pixels that are lower than min_val 

    H, W = img.shape
    output = np.zeros((H, W))

    edge_start_point_y, edge_start_point_x = np.nonzero(img > max_val)  # get all pixels that are beginnings of an edge
    point_num = edge_start_point_y.shape[0]
    for i in range(point_num):
        output[edge_start_point_y[i], edge_start_point_x[i]] = 1    # never forget x axis corresponds to columns
        # use BFS to quickly extend edges
        queue = deque()
        queue.append((edge_start_point_y[i], edge_start_point_x[i]))
        visited = np.zeros((H, W))
        while(queue):
            y, x = queue.popleft()
            visited[y, x] = 1
            for dy in [-1, 0, 1]:
                for dx in [-1, 0, 1]:
                    yy = y + dy
                    xx = x + dx
                    if yy >= 0 and yy < H and xx >= 0 and xx < W:
                        if visited[yy, xx] == 0:
                            visited[yy, xx] = 1
                            if img[yy, xx] >= min_val and img[yy, xx] < max_val:
                                output[yy, xx] = 1
                                queue.append((yy, xx))
    return output



if __name__=="__main__":

    # Load the input images
    input_img = read_img("Lenna.png") / 255

    # Apply gaussian blurring
    blur_img = Gaussian_filter(input_img)

    x_grad = Sobel_filter_x(blur_img)
    y_grad = Sobel_filter_y(blur_img)

    # Compute the magnitude and the direction of gradient
    magnitude_grad, direction_grad = compute_gradient_magnitude_direction(x_grad, y_grad)

    # NMS
    NMS_output = non_maximal_suppressor(magnitude_grad, direction_grad)

    # Edge linking with hysteresis
    output_img = hysteresis_thresholding(NMS_output)
    
    write_img("result/HM1_Canny_result.png", output_img * 255)