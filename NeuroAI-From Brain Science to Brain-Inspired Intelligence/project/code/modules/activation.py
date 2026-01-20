from spikingjelly.activation_based.neuron import LIFNode
from spikingjelly.activation_based import surrogate
from .my_surrogate import MySigmoid, MySuperSpike, MyTriangular

surrogate_dict = {
    'sigmoid': surrogate.Sigmoid,
    'my_sigmoid': MySigmoid,
    'my_superspike': MySuperSpike,
    'my_triangular': MyTriangular,
}

class LIF(LIFNode):
    def __init__(self, tau=2.0, surrogate_function='sigmoid', alpha=4.0,
                 step_mode='m', decay_input=False, 
                 v_threshold=1.0, v_reset=None, detach_reset=True
                 ):

        super().__init__(float(tau), decay_input, v_threshold, v_reset,   # type: ignore
                         surrogate_function=surrogate_dict[surrogate_function](alpha),
                         detach_reset=detach_reset, step_mode=step_mode)
        
    def forward(self, x):
        # print(self.surrogate_function)
        spike = super().forward(x)
        return spike
        