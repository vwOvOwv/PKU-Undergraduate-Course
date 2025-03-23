import numpy as np
from utils import draw_save_plane_with_points, normalize


if __name__ == "__main__":


    np.random.seed(0)
    # load data, total 130 points inlcuding 100 inliers and 30 outliers
    # to simplify this problem, we provide the number of inliers and outliers here

    noise_points = np.loadtxt("HM1_ransac_points.txt")


    #RANSAC
    # Please formulate the palnace function as:  A*x+B*y+C*z+D=0     

    sample_time = # the minimal time that can guarantee the probability of at least one hypothesis does not contain any outliers is larger than 99.9%
    distance_threshold = 0.05

    # sample points group




    # estimate the plane with sampled points group




    #evaluate inliers (with point-to-plance distance < distance_threshold)



    # minimize the sum of squared perpendicular distances of all inliers with least-squared method 



    # draw the estimated plane with points and save the results 
    # check the utils.py for more details
    # pf: [A,B,C,D] contains the parameters of palnace function  A*x+B*y+C*z+D=0  
    pf = normalize(pf)
    draw_save_plane_with_points(pf, noise_points,"result/HM1_RANSAC_fig.png") 
    np.savetxt("result/HM1_RANSAC_plane.txt", pf)
    np.savetxt('result/HM1_RANSAC_sample_time.txt', np.array([sample_time]))
