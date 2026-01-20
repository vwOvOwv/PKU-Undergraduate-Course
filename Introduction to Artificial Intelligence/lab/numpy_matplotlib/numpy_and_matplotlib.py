import argparse
import numpy as np
import matplotlib.pyplot as plt

'''
本次实验的目的是为了熟悉numpy对矩阵的操作以及matplotlib的使用
'''

def get_top_k(U, S, V, k):
    '''
    获取矩阵的前k列/行
    输入：
        U: 形状为(244, 207)的矩阵
        S: 形状为(207,)的矩阵
        V: 形状为(207, 207)的矩阵
    输出：
        U: 形状为(244, k)的矩阵
        S: 形状为(k,)的矩阵
        V: 形状为(k, 207)的矩阵
    '''
    # TODO: 修改U,S,V的形状，使其满足输出要求
    new_U, new_S, new_V = U, S, V
    new_U=U[:244,:k]
    new_S=S[:k]
    new_V=V[:k]
    return new_U, new_S, new_V


def plot_S(S):
    '''
    S是一个向量，其元素为S1, S2, S3, ..., Sn，并且S1 >= S2 >= S3 >= ... >= Sn
    我们需要计算一个新的向量E，其含义为前i个元素的平方和占总平方和的比例，即
    Ei = (S1^2 + S2^2 + ... + Si^2) / (S1^2 + S2^2 + ... + Sn^2)
    '''
    # TODO 你需要利用S计算出E，并且画出E随i的变化的曲线
    # n=len(S)
    # sum=0
    # for i in range(n):
    #     sum+=S[i]**2
    # sigma=0
    # E=[]
    # for i in range(n):
    #     sigma+=S[i]**2
    #     tmp=sigma/sum
    #     E.append(tmp)
    n=len(S)
    X=[i for i in range(n)]
    E=np.cumsum(S**2)/np.sum(S**2)
    plt.title("E-i")
    plt.xlabel('i')
    plt.ylabel('E')
    plt.plot(X,E)
    plt.show()
    # print(E)
    # print(S)
    return 

def matmul_U_S_V(U, S, V):
    '''
    使用矩阵乘法，将U,S,V相乘，得到一个新的矩阵
    注意S目前是一个向量，需要转化为一个对角矩阵DIAG(S),其对角线上的元素为S
    返回值为 U*DIAG(S)*V, 此处*表示矩阵乘法
    
    Tips:
        可以使用np.diag函数，当其输入为一个向量时，会将其转化为对角矩阵
    注意：
        np.diag函数如果输入的是一个矩阵，会将其对角线上的元素提取出来，请注意区分
    '''
    # TODO 使用矩阵乘法，将U,S,V相乘，得到一个新的矩阵
    new_mat = np.zeros((U.shape[0], V.shape[1]))
    S=np.diag(S)
    new_mat=np.dot(U,S)
    new_mat=np.dot(new_mat,V)
    return new_mat



def task1(U, S, V):
    '''
    任务1：请完成并使用get_top_k函数，获取U,S,V的前k列/行
    '''
    k = 4
    new_U, new_S, new_V = get_top_k(U, S, V, k)
    print('新的USV形状为: ', new_U.shape, new_S.shape, new_V.shape)
    # 新的USV形状为:  (244, 4) (4,) (4, 207)
    pass

def task2(U, S, V):
    '''
    完成并使用plot_S函数，将S的占比分布变化图画出来
    '''
    plot_S(S)
    pass

def task3(U, S, V):
    '''
    任务3：请完成并使用matmul_U_S_V函数，将U,S,V相乘，得到一个新的矩阵
    之后使用plt.imshow函数，将矩阵以灰度图的形式显示出来
    输出参考task3.png
    '''
    k = 2
    new_U, new_S, new_V = get_top_k(U, S, V, k)
    mat = matmul_U_S_V(new_U, new_S, new_V)
    plt.imshow(mat, cmap='gray')
    plt.show()


def task4(U, S, V):
    '''
    任务4: 你可以尝试不断调整task3中k的值，观察灰度图的变化
    但是一个更简单的方法是我们可以用子图的方式，将不同k值的灰度图画在一张图上
    你的任务是看懂下面的代码，并且运行，观察灰度图的变化
    '''
    
    k_list = [1, 4, 16, 64]
    curve = np.cumsum(S**2) / np.sum(S**2)
    
    for i, k in enumerate(k_list):
        
        plt.subplot(2, 2, i+1, title='k={}, eta={:.2f}'.format(k, curve[k]), xticks=[], yticks=[]) 
        # 画子图， 2,2表示将画布分为2行2列，i+1表示现在画第i+1个子图(从1开始)
        # title表示子图的标题，xticks和yticks表示子图的x轴和y轴的刻度(这里我们不需要刻度所以设置为空)
        
        new_U, new_S, new_V = get_top_k(U, S, V, k)
        mat = matmul_U_S_V(new_U, new_S, new_V)
        plt.imshow(mat, cmap='gray')

    plt.show()
    pass

# 此处类似于main函数，当我们运行这个文件的时候，会执行下面的代码
if __name__ == '__main__':
    
    # 运行指令
    # python test.p-y --task task1 
    # python test.py --task task2
    # 以此类推
    
    # 使用argparse模块，可以方便的获取命令行参数
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument('--task', type=str, default='task1', help='task name')
    args = arg_parser.parse_args()
    
    
    #我们提供了一个data.npy文件，里面包含了U,S,V三个numpy矩阵，我们首先读取这个文件，并且打印出U,S,V的shape

    U, S, V = np.load('data.npy', allow_pickle=True)
    print(U.shape, S.shape, V.shape)
    # 打印结果是 (244, 207) (207,) (207, 207)

    task_name = args.task
    globals()[task_name](U,S,V) # 这里的globals()[task_name]相当于调用了task1函数，如果task_name='task2'，则会调用task2函数