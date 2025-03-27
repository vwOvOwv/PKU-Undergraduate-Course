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

    H_in, W_in = (img.shape[0], img.shape[1])   # input image is a 2-D array
    H_out, W_out = (H_in + 2 * padding_size, W_in + 2 * padding_size)
    padding_img = np.zeros(shape=(H_out, W_out))
    padding_img[padding_size: H_out - padding_size, 
                    padding_size: W_out - padding_size] = img.copy()
    if type=="zeroPadding":
        return padding_img
    elif type=="replicatePadding":
        # padding four corners
        padding_img[0: padding_size,
                    0: padding_size] = img[0, 0]
        padding_img[0: padding_size, 
                    W_out - padding_size: W_out] = img[0, W_in - 1]
        padding_img[H_out - padding_size: H_out,
                    0: padding_size] = img[H_in - 1, 0]
        padding_img[H_out - padding_size: H_out,
                    W_out - padding_size: W_out] = img[H_in - 1, W_in - 1]
        
        # padding four edges
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
    # zero padding (stride = 1, keep output shape same as input shape)
    padding_size = int((kernel.shape[0] - 1) / 2)
    padding_img = padding(img=img, padding_size=padding_size, type="zeroPadding")

    # build the Toeplitz matrix and compute convolution
    n_row = img.shape[0] * img.shape[1]
    n_col = padding_img.shape[0] * padding_img.shape[1]
    vectorized_img = padding_img.reshape(-1, 1, order='F') # in this case 64x1
    
    zeros = np.zeros(shape=(padding_img.shape[0] - kernel.shape[0], kernel.shape[1]))
    concated_kernel = np.concatenate((kernel, zeros), axis=0)
    vectorized_kernel = concated_kernel.reshape(-1, 1, order='F').T
    vectorized_kernel = np.concatenate((vectorized_kernel, 
                                        np.zeros(shape=(1, n_col - vectorized_kernel.shape[1]))),
                                        axis=1)
    toeplitz = np.zeros(shape=(n_row, n_col))
    row_index = np.arange(n_row)[:, None]
    col_index = np.arange(n_col)
    indices = (col_index - row_index) % n_col
    toeplitz = vectorized_kernel.reshape(-1)[indices]
    output = np.matmul(toeplitz, vectorized_img)
    np.savetxt("result/tmp_toeplitz", toeplitz)
    return output.reshape(img.shape[0], img.shape[1], order='F')


# def convolve(img, kernel):
#     """
#         The function you need to implement for Q1 c).
#         Inputs:
#             img: array(float)
#             kernel: array(float)
#         Outputs:
#             output: array(float)
#     """
    
#     #build the sliding-window convolution here
    

#     return output


# def Gaussian_filter(img):
#     padding_img = padding(img, 1, "replicatePadding")
#     gaussian_kernel = np.array([[1/16,1/8,1/16],[1/8,1/4,1/8],[1/16,1/8,1/16]])
#     output = convolve(padding_img, gaussian_kernel)
#     return output

# def Sobel_filter_x(img):
#     padding_img = padding(img, 1, "replicatePadding")
#     sobel_kernel_x = np.array([[-1,0,1],[-2,0,2],[-1,0,1]])
#     output = convolve(padding_img, sobel_kernel_x)
#     return output

# def Sobel_filter_y(img):
#     padding_img = padding(img, 1, "replicatePadding")
#     sobel_kernel_y = np.array([[-1,-2,-1],[0,0,0],[1,2,1]])
#     output = convolve(padding_img, sobel_kernel_y)
#     return output



if __name__=="__main__":

    np.random.seed(111)
    input_array=np.random.rand(6,6)
    input_kernel=np.random.rand(3,3)


    # task1: padding
    zero_pad =  padding(input_array,1,"zeroPadding")
    np.savetxt("result/HM1_Convolve_zero_pad.txt",zero_pad)

    replicate_pad = padding(input_array,1,"replicatePadding")
    np.savetxt("result/HM1_Convolve_replicate_pad.txt",replicate_pad)


    #task 2: convolution with Toeplitz matrix
    result_1 = convol_with_Toeplitz_matrix(input_array, input_kernel)
    np.savetxt("result/HM1_Convolve_result_1.txt", result_1)

    # #task 3: convolution with sliding-window
    # result_2 = convolve(input_array, input_kernel)
    # np.savetxt("result/HM1_Convolve_result_2.txt", result_2)

    # #task 4/5: Gaussian filter and Sobel filter
    # input_img = read_img("lenna.png")/255

    # img_gadient_x = Sobel_filter_x(input_img)
    # img_gadient_y = Sobel_filter_y(input_img)
    # img_blur = Gaussian_filter(input_img)

    # write_img("result/HM1_Convolve_img_gadient_x.png", img_gadient_x*255)
    # write_img("result/HM1_Convolve_img_gadient_y.png", img_gadient_y*255)
    # write_img("result/HM1_Convolve_img_blur.png", img_blur*255)




    