import torch
from spikingjelly.activation_based import surrogate

class Sigmoid(torch.autograd.Function):
    @staticmethod
    def forward(ctx, x, alpha):
        if x.requires_grad:
            ctx.save_for_backward(x)
            ctx.alpha = alpha
        return (x >= 0).float()

    @staticmethod
    def backward(ctx, grad_output): # type: ignore
        grad_x = None
        if ctx.needs_input_grad[0]:
            x, = ctx.saved_tensors
            alpha = ctx.alpha
            
            sigmoid_x = torch.sigmoid(alpha * x)
            sigmoid_grad = alpha * sigmoid_x * (1 - sigmoid_x)
            grad_x = grad_output * sigmoid_grad
            
        return grad_x, None

class MySigmoid(surrogate.SurrogateFunctionBase):
    def __init__(self, alpha=1.0, spiking=True):
        super().__init__(alpha, spiking)

    @staticmethod
    def spiking_function(x, alpha):
        return Sigmoid.apply(x, alpha)
    

class SuperSpike(torch.autograd.Function):
    @staticmethod
    def forward(ctx, x, alpha):
        if x.requires_grad:
            ctx.save_for_backward(x)
            ctx.alpha = alpha
        return (x >= 0).float()

    @staticmethod
    def backward(ctx, grad_output): # type: ignore
        grad_x = None
        if ctx.needs_input_grad[0]:
            x, = ctx.saved_tensors
            alpha = ctx.alpha
            
            denominator = alpha * x.abs() + 1
            h_x = 1 / denominator.pow(2)
            grad_x = grad_output * h_x
            
        return grad_x, None

class MySuperSpike(surrogate.SurrogateFunctionBase):
    def __init__(self, alpha=10.0, spiking=True):
        super().__init__(alpha, spiking)

    @staticmethod
    def spiking_function(x, alpha):
        return SuperSpike.apply(x, alpha)
    

class Triangular(torch.autograd.Function):
    @staticmethod
    def forward(ctx, x, alpha):
        if x.requires_grad:
            ctx.save_for_backward(x)
            ctx.alpha = alpha
        return (x >= 0).float()

    @staticmethod
    def backward(ctx, grad_output): # type: ignore
        grad_x = None
        if ctx.needs_input_grad[0]:
            x, = ctx.saved_tensors
            alpha = ctx.alpha

            h_x = 1.0 - alpha * x.abs()
            h_x = torch.clamp(h_x, min=0)
            grad_x = grad_output * h_x
            
        return grad_x, None

class MyTriangular(surrogate.SurrogateFunctionBase):
    def __init__(self, alpha=10.0, spiking=True):
        super().__init__(alpha, spiking)

    @staticmethod
    def spiking_function(x, alpha):
        return Triangular.apply(x, alpha)