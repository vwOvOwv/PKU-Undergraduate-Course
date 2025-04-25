import numpy as np
from utils import read_img, write_img

def padding(img:np.ndarray, padding_size:int, type:str):
    """
        The function you need to implement for Q1 a).
        Inputs:
            img: array(float)
            padding_size: int
            type: str, zeroPadding/replicatePadding
        Outputs:
            padding_img: array(float)
    """
    # A general padding function for 2-D images
    H_in, W_in = (img.shape[0], img.shape[1])
    H_out, W_out = (H_in + 2 * padding_size, W_in + 2 * padding_size)
    padding_img = np.zeros(shape=(H_out, W_out))
    padding_img[padding_size: H_out - padding_size, 
                    padding_size: W_out - padding_size] = img
    if type=="zeroPadding":
        return padding_img
    elif type=="replicatePadding":
        # replicate four corners
        padding_img[0: padding_size,
                    0: padding_size] = img[0, 0]
        padding_img[0: padding_size, 
                    W_out - padding_size: W_out] = img[0, W_in - 1]
        padding_img[H_out - padding_size: H_out,
                    0: padding_size] = img[H_in - 1, 0]
        padding_img[H_out - padding_size: H_out,
                    W_out - padding_size: W_out] = img[H_in - 1, W_in - 1]
        
        # replicate four edges
        padding_img[padding_size: H_out - padding_size,
                    0: padding_size] = img[:, 0].reshape(-1, 1)
        padding_img[padding_size: H_out - padding_size,
                    W_out - padding_size: W_out] = img[:, W_in - 1].reshape(-1, 1)
        padding_img[0: padding_size,
                    padding_size: W_out - padding_size] = img[0, :].reshape(1, -1)
        padding_img[H_out - padding_size: H_out,
                    padding_size: W_out - padding_size] = img[H_in - 1, :].reshape(1, -1)
        return padding_img


def convol_with_Toeplitz_matrix(img:np.ndarray, kernel:np.ndarray):
    """
        The function you need to implement for Q1 b).
        Inputs:
            img: array(float) 6*6
            kernel: array(float) 3*3
        Outputs:
            output: array(float)
    """
    # zero padding
    padding_size = int((kernel.shape[0] - 1) / 2)   # keep output shape same as input shape
    padding_img = padding(img, padding_size, "zeroPadding")

    # build the Toeplitz matrix and compute convolution
    """
    Toeplitz matrix:
        (H_out * W_out) x (padding_img.shape[0] * padding_img.shape[1])
    
    Each row contains all the kernel weights at specific positions (contains 0s 
    in other positions), mapping multiple pixels from the padding image to the 
    corresponding pixel in the output image.
    """ 
    n_row = img.shape[0] * img.shape[1] # output shape is the same as input shape
    n_col = padding_img.shape[0] * padding_img.shape[1]
    vectorized_img = padding_img.reshape(-1, 1, order='F') # reshape along each column, (padding_img.shape[0] * padding_img.shape[1]) x 1
    
    zeros = np.zeros(shape=(
        padding_img.shape[0] - kernel.shape[0], kernel.shape[1]
        ))
    concated_kernel = np.concatenate((kernel, zeros), axis=0)   # padding_img.shape[0] x kernel.shape[1]
    vectorized_kernel = concated_kernel.reshape(-1, 1, order='F').T # 1 x (padding_img.shape[0] * kernel.shape[1])
    vectorized_kernel = np.concatenate((vectorized_kernel, 
                                        np.zeros(shape=(1, n_col - vectorized_kernel.shape[1]))),
                                        axis=1) # 1 x (padding_img.shape[0] * padding_img.shape[1])
    """
    if input image a_{ij} is a 6 x 6 image and the kernel is  [1 2 3
                                                               4 5 6
                                                               7 8 9]
    then `vectorized_kernel` is:
    [1 4 7 0 0 0 0 0 | 2 5 8 0 0 0 0 0 | 3 6 9 0 0 0 0 0 | 0 0 0 ... 0]

    and `vectorized_img` is:
    [a_{00}, a_{10}, a_{20}, ..., a_{70}, a_{01}, a_{02}, ..., a_{77}]^T
    """
    # First build a circular matrix using vectorized_kernel
    toeplitz = np.zeros(shape=(n_row, n_col))
    row_index = np.arange(n_row)[:, None]
    col_index = np.arange(n_col)
    indices = (col_index - row_index) % n_col
    toeplitz = vectorized_kernel.reshape(-1)[indices]   # (H_out * W_out) x (padding_img.shape[0] * padding_img.shape[1])
    """
    `toeplitz` is now a circular matrix, where each row shifts by one element 
    to the right:
    [[1 4 7 0 0 0 0 0 | 2 5 8 0 0 0 0 0 | 3 6 9 0 0 0 0 0 | 0 0 0 ... 0]
     [0 1 4 6 0 0 0 0 | 0 2 5 8 0 0 0 0 | 0 3 6 9 0 0 0 0 | 0 0 0 ... 0]
     ...]]

    So far, I am trying to build an image-shape-independent function for all 
    2D images, rather than for a specific 6x6 image. However, it seems quite 
    challenging to handle positions in `vectorized_img`
    such as [..., a_{60}, a_{70}, a_{01}, ...], 
    where `toeplitz` needs to be shifted two additional elements.
    """
    toeplitz[6:] = np.roll(toeplitz[6:], shift=2, axis=1)
    toeplitz[12:] = np.roll(toeplitz[12:], shift=2, axis=1)
    toeplitz[18:] = np.roll(toeplitz[18:], shift=2, axis=1)
    toeplitz[24:] = np.roll(toeplitz[24:], shift=2, axis=1)
    toeplitz[30:] = np.roll(toeplitz[30:], shift=2, axis=1)

    output = np.matmul(toeplitz, vectorized_img)    # H_out x W_out
    return output.reshape(img.shape[0], img.shape[1], order='F')


def convolve(img: np.ndarray, kernel: np.ndarray):
    """
        The function you need to implement for Q1 c).
        Inputs:
            img: array(float)
            kernel: array(float)
        Outputs:
            output: array(float)
    """
    
    # build the sliding-window convolution here
    H_in, W_in = img.shape
    kernel_size, _ = kernel.shape
    H_out, W_out = H_in - kernel_size + 1, W_in - kernel_size + 1

    """
    The basic idea is to determine which pixels in the input image (`img`) 
    contribute to each pixel in the output image (`output`). It is challenging 
    to utilize broadcasting effectively to construct a blocked matrix that 
    captures this mapping information.

    Note that `output[i, j]` comes from `img[i: i + kernel_size, j: j + kernelsize]`
    """
    row_index, col_index = np.meshgrid(np.arange(H_out), np.arange(W_out), indexing='ij')   # H_out x W_out
    delta_row, delta_col = np.meshgrid(np.arange(kernel_size), np.arange(kernel_size), indexing='ij') # H_out x W_out

    row_index = row_index[:, :, np.newaxis, np.newaxis] + delta_row[np.newaxis, np.newaxis, :, :]
    col_index = col_index[:, :, np.newaxis, np.newaxis] + delta_col[np.newaxis, np.newaxis, :, :]
    """
    Now, `row_index` and `col_index` is both a 
    `H_out` x `W_out` x `kernel_size` x `kernel_size` matrix. 
    Element `[i, j, :, :]` records where the `output[i, j]` comes from.
    """
    image_matrix = img[row_index, col_index]    # H_out x W_out x kernel_size x kernel_size
    output = np.matmul(image_matrix.reshape(-1, kernel_size ** 2), kernel.reshape(-1, 1))
    output = output.reshape(H_out, W_out)
    return output


def Gaussian_filter(img:np.ndarray):
    padding_img = padding(img, 1, "replicatePadding")
    gaussian_kernel = np.array([[1/16, 1/8, 1/16], 
                                [1/8, 1/4, 1/8], 
                                [1/16, 1/8, 1/16]])
    output = convolve(padding_img, gaussian_kernel)
    return output

def Sobel_filter_x(img:np.ndarray):
    padding_img = padding(img, 1, "replicatePadding")
    sobel_kernel_x = np.array([[-1, 0, 1], 
                               [-2, 0, 2], 
                               [-1, 0, 1]])
    output = convolve(padding_img, sobel_kernel_x)
    return output

def Sobel_filter_y(img:np.ndarray):
    padding_img = padding(img, 1, "replicatePadding")
    sobel_kernel_y = np.array([[-1,-2,-1],[0,0,0],[1,2,1]])
    output = convolve(padding_img, sobel_kernel_y)
    return output



if __name__=="__main__":

    np.random.seed(111)
    input_array = np.random.rand(6, 6)
    input_kernel = np.random.rand(3, 3)

    # task1: padding
    zero_pad = padding(input_array, 1, "zeroPadding")
    np.savetxt("result/HM1_Convolve_zero_pad.txt", zero_pad)

    replicate_pad = padding(input_array, 1, "replicatePadding")
    np.savetxt("result/HM1_Convolve_replicate_pad.txt", replicate_pad)

    # task 2: convolution with Toeplitz matrix
    result_1 = convol_with_Toeplitz_matrix(input_array, input_kernel)
    np.savetxt("result/HM1_Convolve_result_1.txt", result_1)

    # task 3: convolution with sliding-window
    result_2 = convolve(input_array, input_kernel)
    np.savetxt("result/HM1_Convolve_result_2.txt", result_2)
    
    # task 4/5: Gaussian filter and Sobel filter
    input_img = read_img("Lenna.png") / 255

    img_gadient_x = Sobel_filter_x(input_img)
    img_gadient_y = Sobel_filter_y(input_img)
    img_blur = Gaussian_filter(input_img)

    write_img("result/HM1_Convolve_img_gadient_x.png", img_gadient_x * 255)
    write_img("result/HM1_Convolve_img_gadient_y.png", img_gadient_y * 255)
    write_img("result/HM1_Convolve_img_blur.png", img_blur * 255)