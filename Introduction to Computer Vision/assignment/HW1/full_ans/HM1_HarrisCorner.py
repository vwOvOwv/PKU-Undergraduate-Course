import numpy as np
from utils import read_img, draw_corner
from HM1_Convolve import convolve, Sobel_filter_x, Sobel_filter_y, padding
from utils import read_img, write_img

def corner_response_function(input_img, window_size, alpha, threshold):
    """
        The function you need to implement for Q3.
        Inputs:
            input_img: array(float)
            window_size: int
            alpha: float
            threshold: float
        Outputs:
            corner_list: array
    """

    # please solve the corner_response_function of each window,
    # and keep windows with theta > threshold.
    # you can use several functions from HM1_Convolve to get 
    # I_xx, I_yy, I_xy as well as the convolution result.
    # for details of corner_response_function, please refer to the slides.

    H, W = input_img.shape

    # get image derivatives
    x_grad = Sobel_filter_x(input_img)
    y_grad = Sobel_filter_y(input_img)
    xy_grad = x_grad * y_grad

    # square of derivatives
    x_grad_square = x_grad ** 2
    y_grad_square = y_grad ** 2

    # convolve with rectangular window
    padding_size = int((window_size - 1) / 2)   # keep invariant image size
    padding_x_grad_square = padding(x_grad_square, padding_size, "replicatePadding")
    padding_y_grad_square = padding(y_grad_square, padding_size, "replicatePadding")
    padding_xy_grad = padding(xy_grad, padding_size, "replicatePadding")
    
    kernel = np.ones((window_size, window_size))
    convolved_x_grad_square = convolve(padding_x_grad_square, kernel)
    convolved_y_grad_square = convolve(padding_y_grad_square, kernel)
    convolved_xy_grad = convolve(padding_xy_grad, kernel)

    # calculate corner response function
    theta = convolved_x_grad_square * convolved_y_grad_square - \
            convolved_xy_grad ** 2 - \
            alpha * (convolved_x_grad_square + convolved_y_grad_square) ** 2 -\
            threshold

    # thresholding
    corner_row_index, corner_col_index = np.nonzero(np.clip(theta, 0, None))
    corner_row_index = corner_row_index[np.newaxis, :]
    corner_col_index = corner_col_index[np.newaxis, :]
    corner_list = np.concatenate((corner_row_index, corner_col_index, 
                                  theta[corner_row_index, corner_col_index]),
                                  axis=0).T

    return corner_list # array, each row contains information about one corner, namely (index of row, index of col, theta)

if __name__=="__main__":

    #Load the input images
    input_img = read_img("hand_writting.png") / 255.

    # you can adjust the parameters to fit your own implementation 
    window_size = 5
    alpha = 0.04
    threshold = 10

    corner_list = corner_response_function(input_img, window_size, alpha, threshold)

    # NMS
    corner_list_sorted = sorted(corner_list, key = lambda x: x[2], reverse = True)
    NML_selected = [] 
    NML_selected.append(corner_list_sorted[0][:-1])
    dis = 10
    for i in corner_list_sorted :
        for j in NML_selected :
            if(abs(i[0] - j[0]) <= dis and abs(i[1] - j[1]) <= dis) :
                break
        else :
            NML_selected.append(i[:-1])

    #save results
    draw_corner("hand_writting.png", "result/HM1_HarrisCorner.png", NML_selected)
