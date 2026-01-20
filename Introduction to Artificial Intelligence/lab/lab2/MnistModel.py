import numpy as np
import modelLogisticRegression as LR
import modelTree as Tree
import modelRandomForest as Forest
import modelSoftmaxRegression as SR
import modelMultiLayerPerceptron as MLP
import pickle

class NullModel:

    def __init__(self):
        pass

    def __call__(self, figure):
        return 0


class LRModel:
    def __init__(self) -> None:
        with open(LR.save_path, "rb") as f:
            self.weight, self.bias = pickle.load(f)

    def __call__(self, figure):
        pred = figure @self.weight + self.bias
        return 0 if pred > 0 else 1

class TreeModel:
    def __init__(self) -> None:
        with open(Tree.save_path, "rb") as f:
            self.root = pickle.load(f)
    
    def __call__(self, figure):
        return Tree.inferTree(self.root, Tree.discretize(figure.flatten()))


class ForestModel:
    def __init__(self) -> None:
        with open(Forest.save_path, "rb") as f:
            self.roots = pickle.load(f)
    
    def __call__(self, figure):
        return Forest.infertrees(self.roots, Forest.discretize(figure.flatten()))


class SRModel:
    def __init__(self) -> None:
        with open(SR.save_path, "rb") as f:
            graph = pickle.load(f)
        self.graph = graph
        self.graph.eval()

    def __call__(self, figure):
        self.graph.flush()
        pred = self.graph.forward(figure, removelossnode=True)[-1]
        return np.argmax(pred, axis=-1)
    
class MLPModel:
    def __init__(self) -> None:
        with open(MLP.save_path, "rb") as f:
            graph = pickle.load(f)
        self.graph = graph
        self.graph.eval()

    def __call__(self, figure):
        self.graph.flush()
        pred = self.graph.forward(figure, removelossnode=True)[-1]
        return np.argmax(pred, axis=-1)

modeldict = {
    "Null": NullModel,
    "LR": LRModel,
    "Tree": TreeModel,
    "Forest": ForestModel,
    "SR": SRModel,
    "MLP": MLPModel,
    "Your": MLPModel
}

