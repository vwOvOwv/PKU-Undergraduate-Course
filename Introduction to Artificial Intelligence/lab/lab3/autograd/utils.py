from .BaseGraph import Graph
from .BaseNode import *

def buildgraph(expressions):
    nodes = []
    for expression in expressions:
        varname, formula = expression[0], expression[1]
        cmd = f"{varname} = {formula}"
        exec(cmd) # 通过exec使用python自己的语法来解析表达式
        if isinstance(eval(varname), Node):
            nodes.append(eval(varname))
    graph = Graph(nodes)
    return graph, nodes

def parse(filename):
    with open(filename, "r") as f:
        expressions = [list(line.strip().replace(" ", "").split("=", maxsplit=1)) for line in f.readlines()]
    varnames = list(map(lambda tp: tp[0], expressions))
    graph, nodes = buildgraph(expressions)
    return varnames, graph, nodes, expressions


class PermIterator:

    def __init__(self, n: int, batch_size: int):
        self.n = n
        self.batch_size = batch_size
        self.idx = 0

    def __iter__(self):
        self.perm = np.random.permutation(self.n)
        self.idx = 0
        return self
    
    def __next__(self):
        if self.idx >= self.n:
            raise StopIteration
        nidx = self.idx + self.batch_size
        ret = self.perm[self.idx: nidx]
        self.idx = nidx
        return ret