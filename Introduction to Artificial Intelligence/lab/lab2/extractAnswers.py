import zipfile
f = zipfile.ZipFile("answer.zip", "r", zipfile.ZIP_DEFLATED)
f.extractall()
f.close()