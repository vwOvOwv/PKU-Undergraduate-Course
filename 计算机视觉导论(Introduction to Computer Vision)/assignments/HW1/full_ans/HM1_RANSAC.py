import numpy as np
from utils import draw_save_plane_with_points, normalize


if __name__ == "__main__":


    np.random.seed(0)
    # load data, total 130 points inlcuding 100 inliers and 30 outliers
    # to simplify this problem, we provide the number of inliers and outliers here

    noise_points = np.loadtxt("HM1_ransac_points.txt")

    # RANSAC
    # Please formulate the palnace function as:  A*x+B*y+C*z+D=0     
    """
    Not np.random.choice(replace=True) means each element can be selected multiple times.
    Thus, 
    1 - (1 - (\frac{10}{13})^3)^k > 0.999 => k \ge 12
    (Most rigorous implementation could be sampling from \textbf{all} points
    for `sample_time` times, each time sampling 3 \textbf{different} points.)
    """
    sample_time = 12 # the minimal time that can guarantee the probability of at least one hypothesis does not contain any outliers is larger than 99.9%
    distance_threshold = 0.05

    # sample points group
    point_num = noise_points.shape[0]   # 130
    noise_points = np.concatenate((noise_points, np.ones(shape=(point_num, 1))), axis=1) # add a column to leverage SVD
    sample_index = np.random.choice(point_num, 3 * sample_time, replace=True)
    samples = noise_points[sample_index, :]
    list_samples = np.array_split(samples, sample_time, axis=0) # divide samples into groups

    # estimate the plane with sampled points group
    # directly use SVD to get the plane equation
    _, _, Vh = np.linalg.svd(list_samples, full_matrices=True, compute_uv=True) # Vh: (12, 4, 4)
    solutions = Vh[:, 3, :]

    # evaluate inliers (with point-to-plane distance < distance_threshold)
    repeated_noise_points = noise_points[np.newaxis, :]
    repeated_noise_points = repeated_noise_points.repeat(sample_time, axis=0)   # (12, 130, 4)
    repeated_solutions = solutions[:, np.newaxis, :].repeat(point_num, axis=1)  # (12, 130, 4)

    # calculate each-point-to-each-plane distance
    denominator = np.sqrt((repeated_solutions[:, :, 0: 3] ** 2).sum(axis=2))    # sqrt(A^2 + B^2 + C^2)
    distances = np.abs(np.sum((repeated_noise_points * repeated_solutions), axis=2)) / denominator
    inlier_mask = (distances < distance_threshold)
    
    # minimize the sum of squared perpendicular distances of all inliers with least-squared method
    # note it is possible to get multiple planes that have most inliers
    inlier_num = inlier_mask.astype(int).sum(axis=1)
    max_inlier_num = np.max(inlier_num, axis=0)
    best_model_mask = (inlier_num == max_inlier_num)
    best_model_num = best_model_mask.astype(int).sum()

    # this time apply SVD on all inliers for each plane that has most inliers
    best_model_inliers = repeated_noise_points[best_model_mask] * inlier_mask.astype(int)[best_model_mask, :, np.newaxis] # (5, 130, 4)
    best_model_inliers = best_model_inliers[np.nonzero(best_model_inliers)]
    best_model_inliers = best_model_inliers.reshape(best_model_num, max_inlier_num, 4)  # (5, 103, 4)
    list_inliers = np.array_split(best_model_inliers, best_model_num, axis=0)
    _, _, Vh = np.linalg.svd(list_inliers, full_matrices=True, compute_uv=True) # Vh: (5, 1, 4, 4)
    solutions = Vh[:, 0, 3, :]  # (5, 4)

    # calculate distances again to select the best model
    repeated_solutions = solutions[:, np.newaxis, :].repeat(max_inlier_num, axis=1) # (5, 103, 4)
    denominator = np.sqrt((repeated_solutions[:, :, 0: 3] ** 2).sum(axis=2))
    distances = np.abs(np.sum((best_model_inliers * repeated_solutions), axis=2)) / denominator
    square_distance_error = np.sum(distances ** 2, axis=1)
    pf = solutions[np.argmin(square_distance_error, axis=0), :]
    
    # draw the estimated plane with points and save the results
    # check the utils.py for more details
    # pf: [A,B,C,D] contains the parameters of palnace function  A*x+B*y+C*z+D=0  
    pf = normalize(pf)
    draw_save_plane_with_points(pf, noise_points, "result/HM1_RANSAC_fig.png") 
    np.savetxt("result/HM1_RANSAC_plane.txt", pf)
    np.savetxt('result/HM1_RANSAC_sample_time.txt', np.array([sample_time]))
