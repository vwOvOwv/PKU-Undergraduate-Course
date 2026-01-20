import numpy as np

# 超参数
# TODO: You can change the hyperparameters here
lr = 1.2 # 学习率
wd = 1e-4  # l2正则化项系数


def predict(X, weight, bias):
    """
    使用输入的weight和bias预测样本X是否为数字0
    @param X: n*d 每行是一个输入样本。n: 样本数量, d: 样本的维度
    @param weight: d
    @param bias: 1
    @return: n wx+b
    """
    # TODO: YOUR CODE HERE
    ans=[]
    n=X.shape[0]
    for i in range(n):
        ans.append(weight@X[i]+bias[0])
    ans=np.array(ans)
    return ans
    raise NotImplementedError

def sigmoid(x):
    if x<-500:#防止溢出
        x=-500
    return 1 / (np.exp(-x) + 1)


def step(X, weight, bias, Y):
    """
    单步训练, 进行一次forward、backward和参数更新
    @param X: n*d 每行是一个训练样本。 n: 样本数量， d: 样本的维度
    @param weight: d
    @param bias: 1
    @param Y: n 样本的label, 1表示为数字0, -1表示不为数字0
    @return:
        haty: n 模型的输出, 为正表示数字为0, 为负表示数字不为0
        loss: 1 由交叉熵损失函数计算得到
        weight: d 更新后的weight参数
        bias: 1 更新后的bias参数
    """
    # TODO: YOUR CODE HERE
    n=X.shape[0]
    d=X.shape[1]
    loss=0
    for i in range(n):
        loss+=np.log(1/(sigmoid(Y[i]*(weight@X[i]+bias[0]))))
    loss=loss/n+wd*weight@weight.T
    partial_w=0
    partial_b=0
    for i in range(n):
        partial_w+=(1-sigmoid(Y[i]*(weight@X[i]+bias[0])))*Y[i]*X[i]
        partial_b+=(1-sigmoid(Y[i]*(weight@X[i]+bias[0])))*Y[i]
    partial_w=-partial_w/n+2*wd*weight
    partial_b=-partial_b/n
    weight=weight-lr*partial_w
    bias[0]=bias[0]-lr*partial_b
    haty=predict(X,weight,bias)
    return haty,loss,weight,bias
    raise NotImplementedError
