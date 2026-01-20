'''
对神经网络进行反向传播
'''
import numpy as np
from numpy.random import randn
from autograd.utils import parse
import argparse
import pickle

parser = argparse.ArgumentParser()
parser.add_argument("filename")
parser.add_argument("--debug", action="store_true")
parser.add_argument("--save", action="store_true")
parser.add_argument("--load", action="store_true")
args = parser.parse_args()


debug = args.debug

filename = args.filename
varnames, graph, nodes, expressions = parse(filename)
rootname = varnames[-1]
X = np.random.randn(100, 10)
params = graph.parameters()
savefilename = filename.replace(".in", ".npy")
if args.save:
    with open(savefilename, "wb") as f:
        pickle.dump([X] + params + [graph[-1].y], f)
if args.load:
    with open(savefilename, "rb") as f:
        saved_paramlist = pickle.load(f)
    X[:] = saved_paramlist[0]
    for i in range(len(params)):
        params[i][:] = saved_paramlist[i+1][:]
    graph[-1].y[:] = saved_paramlist[-1]

value = graph.forward(X, debug)[-1]
graph.eval()
graph.flush()
value = graph.forward(X, debug)[-1]
xgrad = graph.backward(debug=debug)

params = graph.parameters()
grads = graph.grads()

print("numerical grad, backward grad")
t = 1e-6

dxsavefilename = filename.replace(".in", ".dx.npy")
dxlist = []

if args.load:
    with open(dxsavefilename, "rb") as f:
        dxlist = pickle.load(f)

graph.flush()
dx = randn(*X.shape)
dxlist.append(dx)
if args.load:
    dx[:] = dxlist[0]
X += t*dx
newvalue = graph.forward(X)[-1]
print((newvalue - value) / t, np.sum(np.multiply(dx, xgrad)))
X -= t * dx

for i, param in enumerate(params):
    graph.flush()
    dx = randn(*grads[i].shape)
    # print(dx)
    if args.load:
        dx[:] = dxlist[i+1]
    dxlist.append(dx)
    param += t * dx
    newvalue = graph.forward(X)[-1]
    print((newvalue - value) / t, np.sum(np.multiply(dx, grads[i])))
    param -= t * dx

if args.save:
    with open(dxsavefilename, "wb") as f:
        pickle.dump(dxlist, f)