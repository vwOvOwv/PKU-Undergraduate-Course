from typing import List
import math
import numpy as np
import numpy as np
from .Init import * 

def shape(x):
    if isinstance(x, np.ndarray):
        ret = "ndarray"
        if np.any(np.isposinf(x)):
            ret += "_posinf"
        if np.any(np.isneginf(x)):
            ret += "_neginf"
        if np.any(np.isnan(x)):
            ret += "_nan"
        return f" {x.shape} "
    if isinstance(x, int):
        return "int"
    if isinstance(x, float):
        ret = "float"
        if np.any(np.isposinf(x)):
            ret += "_posinf"
        if np.any(np.isneginf(x)):
            ret += "_neginf"
        if np.any(np.isnan(x)):
            ret += "_nan"
        return ret
    else:
        raise NotImplementedError(f"unsupported type {type(x)}")

class Node(object):
    def __init__(self, name, *params):
        # 节点的梯度，self.grad[i]对应self.params[i]
        self.grad = []
        # 节点保存的临时数据
        self.cache = []
        # 节点的名字
        self.name = name
        # 用于Linear节点中存储weight和bias参数使用
        self.params = list(params)

    def num_params(self):
        return len(self.params)

    def cal(self, X):
        '''
        计算函数值
        '''
        pass

    def backcal(self, grad):
        '''
        计算梯度
        '''
        pass

    def flush(self):
        # 初始化/刷新
        self.grad = []
        self.cache = []

    def forward(self, x, debug=False):
        '''
        正向传播
        '''
        if debug:
            print(self.name, shape(x))
        ret = self.cal(x)
        if debug:
            print(shape(ret))
        return ret

    def backward(self, grad, debug=False):
        '''
        反向传播
        '''
        if debug:
            print(self.name, shape(grad))
        ret = self.backcal(grad)
        if debug:
            print(shape(ret))
        return ret
    
    def eval(self):
        pass

    def train(self):
        pass


class relu(Node):
    # shape x: (*)
    # shape value: (*) relu(x)
    def __init__(self):
        super().__init__("relu")

    def cal(self, x):
        self.cache.append(x)
        return np.clip(x, 0, None)

    def backcal(self, grad):
        return np.multiply(grad, self.cache[-1] > 0) 

class sigmoid(Node):
    # shape x: (*)
    # shape value: (*) sigmoid(x)
    def __init__(self):
        super().__init__("sigmoid")

    def cal(self, X):
        # TODO: YOUR CODE HERE
        tmp=np.clip(X,-500,None)
        ret=1/(1+np.exp(-tmp))
        self.cache.append(ret)
        return ret
        raise NotImplementedError        

    def backcal(self, grad):
        # TODO: YOUR CODE HERE
        return np.multiply(grad,np.multiply(1-self.cache[-1],self.cache[-1]))
        raise NotImplementedError        
    
class tanh(Node):
    # shape x: (*)
    # shape value: (*) tanh(x)
    def __init__(self):
        super().__init__("tanh")

    def cal(self, x):
        ret = np.tanh(x)
        self.cache.append(ret)
        return ret


    def backcal(self, grad):
        return np.multiply(grad, np.multiply(1+self.cache[-1], 1-self.cache[-1]))
    

class Linear(Node):
    # shape x: (*,d1)
    # shape weight: (d1, d2)
    # shape bias: (d2)
    # shape value: (*, d2) 
    def __init__(self, indim, outdim):
        """
        初始化
        @param indim: 输入维度
        @param outdim: 输出维度
        """
        weight = kaiming_uniform(indim, outdim)
        bias = zeros(outdim)
        super().__init__("linear", weight, bias)
        #print(self.params)

    def cal(self, X):
        # TODO: YOUR CODE HERE
        ret=X@self.params[0]+self.params[1]
        # print("params:",self.params[1])
        # print("X:",shape(X))
        self.cache.append(X)
        return ret
        raise NotImplementedError

    def backcal(self, grad):
        # TODO: YOUR CODE HERE
        # print(self.cache[-1].T@grad)
        # print("grad:",shape(grad))
        self.grad.append(self.cache[-1].T@grad)
        self.grad.append(np.sum(grad,axis=0))
        return grad@self.params[0].T
        raise NotImplementedError


class StdScaler(Node):
    '''
    input shape (*)
    output shape (*)
    '''
    EPS = 1e-3
    def __init__(self, mean, std):
        super().__init__("StdScaler")
        self.mean = mean
        self.std = std

    def cal(self, X):
        X = X.copy()
        X -= self.mean
        X /= (self.std + self.EPS)
        return X

    def backcal(self, grad):
        return grad/ (self.std + self.EPS)
    


class BatchNorm(Node):
    '''
    input shape (*)
    output shape (*)
    '''
    EPS = 1e-3
    def __init__(self, indim, momentum: float = 0.9):
        super().__init__("batchnorm", ones((indim)), zeros(indim))
        self.momentum = momentum
        self.mean = None
        self.std = None
        self.updatemean = True
        self.indim = indim

    def cal(self, X):
        if self.updatemean:
            tmean, tstd = np.mean(X, axis=0, keepdims=True), np.std(X, axis=0, keepdims=True)
            if self.std is None or self.std is None:
                self.mean = tmean
                self.std = tstd
            else:
                self.mean *= self.momentum
                self.mean += (1-self.momentum) * tmean
                self.std *= self.momentum
                self.std += (1-self.momentum) * tstd
        X = X.copy()
        X -= self.mean
        X /= (self.std + self.EPS)
        self.cache.append(X.copy())
        X *= self.params[0]
        X += self.params[1]
        return X

    def backcal(self, grad):
        X = self.cache[-1]
        self.grad.append(np.multiply(X, grad).reshape(-1, self.indim).sum(axis=0))
        self.grad.append(grad.reshape(-1, self.indim).sum(axis=0))
        return (grad*self.params[0])/ (self.std + self.EPS)
    
    def eval(self):
        self.updatemean = False

    def train(self):
        self.updatemean = True


class Dropout(Node):
    '''
    input shape (*)
    output shape (*)
    '''
    def __init__(self, p: float = 0.1):
        super().__init__("dropout")
        assert 0<=p<=1, "p 是dropout 概率，必须在[0, 1]中"
        self.p = p
        self.dropout = True

    def cal(self, X):
        if self.dropout:
            X = X.copy()
            mask = np.random.rand(*X.shape) < self.p
            np.putmask(X, mask, 0)
            self.cache.append(mask)
        else:
            X = X*(1/(1-self.p))
        return X
    
    def backcal(self, grad):
        if self.dropout:
            grad = grad.copy()
            np.putmask(grad, self.cache[-1], 0)
            return grad
        else:
            return (1/(1-self.p)) * grad
    
    def eval(self):
        self.dropout=False

    def train(self):
        self.dropout=True

class Softmax(Node):
    # shape x: (*)
    # shape value: (*), softmax at dim
    def __init__(self, dim=-1):
        super().__init__("softmax")
        self.dim = dim

    def cal(self, X):
        X = X - np.max(X, axis=self.dim, keepdims=True)
        expX = np.exp(X)
        ret = expX / expX.sum(axis=self.dim, keepdims=True)
        self.cache.append(ret)
        return ret

    def backcal(self, grad):
        softmaxX = self.cache[-1]
        grad_p = np.multiply(grad, softmaxX)
        return grad_p - np.multiply(grad_p.sum(axis=self.dim, keepdims=True), softmaxX)


class LogSoftmax(Node):
    # shape x: (*)
    # shape value: (*), logsoftmax at dim 
    def __init__(self, dim=-1):
        super().__init__("logsoftmax")
        self.dim = dim

    def cal(self, X):
        # TODO: YOUR CODE HERE
        ret=np.log(Softmax().cal(X)+1e-6)
        self.cache.append(ret)
        return ret
        raise NotImplementedError

    def backcal(self, grad):
        # TODO: YOUR CODE HERE
        LogSoftmaxX=self.cache[-1]
        return grad-np.multiply(grad.sum(axis=self.dim,keepdims=True),np.exp(LogSoftmaxX))
        raise NotImplementedError




class NLLLoss(Node):
    '''
    negative log-likelihood 损失函数
    '''
    # shape x: (*, d), y: (*)
    # shape value: number 
    # 输入：x: (*) 个预测，每个预测是个d维向量，代表d个类别上分别的log概率。  y：(*) 个整数类别标签
    # 输出：NLL损失
    def __init__(self, y):
        """
        初始化
        @param y: n 样本的label
        """
        super().__init__("NLLLoss")
        self.y = y

    def cal(self, X):
        y = self.y
        self.cache.append(X)
        return - np.sum(
            np.take_along_axis(X, np.expand_dims(y, axis=-1), axis=-1))

    def backcal(self, grad):
        X, y = self.cache[-1], self.y
        ret = np.zeros_like(X)
        np.put_along_axis(ret, np.expand_dims(y, axis=-1), -1, axis=-1)
        return grad * ret



class CrossEntropyLoss(Node):#？
    '''
    多分类交叉熵损失函数，不同于课上讲的二分类。它与NLLLoss的区别仅在于后者输入log概率，前者输入概率。
    '''
    # shape x: (*, d), y: (*)
    # shape value: number 
    # 输入：x: (*) 个预测，每个预测是个d维向量，代表d个类别上分别的概率。  y：(*) 个整数类别标签
    # 输出：交叉熵损失
    def __init__(self, y):
        """
        初始化
        @param y: n 样本的label
        """
        super().__init__("CELoss")
        self.y = y

    def cal(self, X):
        # TODO: YOUR CODE HERE
        # 提示，可以对照NLLLoss的cal
        y = self.y
        self.cache.append(X)
        X=np.log(X+1e-6)
        # print(np.sum(X))
        return - np.sum(
            np.take_along_axis(X, np.expand_dims(y, axis=-1), axis=-1))
        raise NotImplementedError

    def backcal(self, grad):
        # TODO: YOUR CODE HERE
        # 提示，可以对照NLLLoss的backcal
        X, y = self.cache[-1], self.y
        X_=np.take_along_axis(X, np.expand_dims(y, axis=-1), axis=-1)
        ret=np.zeros_like(X)
        for i in range(y.shape[0]):
            ret[i][y[i]]=-1/(X_[i]+1e-6)
        # print(ret)
        return grad * ret
        raise NotImplementedError