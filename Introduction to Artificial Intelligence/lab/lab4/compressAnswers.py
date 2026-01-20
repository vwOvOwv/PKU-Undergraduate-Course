import zipfile

filenames = ["info.yaml", "answerLocalization.py", "answerPDControl.py", "answerPlanning.py"]

f = zipfile.ZipFile("answer.zip", "w", zipfile.ZIP_DEFLATED)
for filename in filenames:
    f.write(filename)
f.close()