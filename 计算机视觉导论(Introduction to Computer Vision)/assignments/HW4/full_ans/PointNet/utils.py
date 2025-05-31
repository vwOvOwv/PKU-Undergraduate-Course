import os
from torch.utils.tensorboard  import SummaryWriter

class setting:
    dataset = "/Users/peiyuliu/Desktop/PKU-Undergraduate-Course-Public/计算机视觉导论(Introduction to Computer Vision)/assignments/HW4/assignment4_ans/data/shapenetcore_partanno_segmentation_benchmark_v0"
    batchSize = 32
    num_points = 1024
    workers = 1
    nepoch = 3
    expf = "exps"
    model = ""
    dataset_type = "shapenet"
    manualSeed = 233


class log_writer:
    def __init__(self, path, log_name ) -> None:

        output_path = os.path.join(path, log_name)
        if not os.path.exists(output_path):
            os.makedirs(output_path)
        self.writer = SummaryWriter(log_dir=output_path)


    def add_train_scalar(self, name, data, n):
        self.writer.add_scalar(name+'/train', data, n)



    def add_test_scalar(self,name, data, n):
        self.writer.add_scalar(name+'/test', data, n)
        


def write_points(finename, points, color, weight):
    point_count=points.shape[0]
    ply_file = open(finename, 'w')
    ply_file.write("ply\n")
    ply_file.write("format ascii 1.0\n")
    ply_file.write("element vertex " + str(point_count) + "\n")
    ply_file.write("property float x\n")
    ply_file.write("property float y\n")
    ply_file.write("property float z\n")

    ply_file.write("property uchar red\n")
    ply_file.write("property uchar green\n")
    ply_file.write("property uchar blue\n")

    ply_file.write("property float weight\n")


    ply_file.write("end_header\n")

    for i in range(point_count):
        ply_file.write(str(points[i, 0]) + " " +
                    str(points[i, 1]) + " " +
                    str(points[i, 2]))

        ply_file.write(" "+str(int(color[i, 2])) + " " +
                        str(int(color[i, 1])) + " " +
                        str(int(color[i, 0])) + " " )

        ply_file.write(str(weight[i]))
        ply_file.write("\n")
    ply_file.close()