import zipfile

filenames = ["autograd/BaseGraph.py", "autograd/BaseNode.py"]
filenames += ["answerLogisticRegression.py", "answerMultiLayerPerceptron.py", "answerRandomForest.py", "answerTree.py"]
filenames += ["info.yaml", "MnistModel.py", "YourTraining.py"]

f = zipfile.ZipFile("answer.zip", "w", zipfile.ZIP_DEFLATED)
for filename in filenames:
    f.write(filename)
f.close()