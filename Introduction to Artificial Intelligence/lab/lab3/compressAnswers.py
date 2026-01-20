import zipfile

filenames = ["info.yaml", "FruitModel.py", 'model/mlp.npy']

f = zipfile.ZipFile("answer.zip", "w", zipfile.ZIP_DEFLATED)
for filename in filenames:
    f.write(filename)
f.close()