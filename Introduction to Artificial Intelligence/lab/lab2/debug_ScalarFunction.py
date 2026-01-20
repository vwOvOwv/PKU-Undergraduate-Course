'''
对标量函数进行反向传播
'''
import math
from copy import deepcopy
from typing import List
from autograd.BaseGraph import Graph
from autograd.utils import parse, buildgraph
import numpy as np
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("filename")
parser.add_argument("--debug", action="store_true")
args = parser.parse_args()


debug = args.debug

filename = args.filename
varnames, graph, nodes, expressions = parse(filename)
rootname = varnames[-1]
print("numerical grad, backward grad")
for x in np.linspace(-3, 3, num=10):
    graph.flush()
    value = graph.forward(x, debug)
    grad2 = graph.backward(debug=debug)
    delta = 1e-6
    graph.flush()
    value1 = graph.forward(x+delta)[-1]
    graph.flush()
    value2 = graph.forward(x-delta)[-1]
    grad1 = (value1-value2)/(2*delta)
    print(grad1, grad2)