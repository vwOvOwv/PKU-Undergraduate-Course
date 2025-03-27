import os
import zipfile

def zipDir(dirpath, outFullName):
    zip = zipfile.ZipFile(outFullName, "w", zipfile.ZIP_DEFLATED)
    for path, dirnames, filenames in os.walk(dirpath):
        # 去掉目标跟路径，只对目标文件夹下边的文件及文件夹进行压缩
        fpath = path.replace(dirpath, '')
 
        for filename in filenames:
            print(filename)
            if filename.endswith('.zip'):
                continue
            zip.write(os.path.join(path, filename), os.path.join(fpath, filename))
    zip.close()
 
 
if __name__ == "__main__":
    
    # ---------------------------------------------------------
    # 请用你的学号和姓名替换下面的内容，注意参照例子的格式，使用拼音而非中文
    id = 2000010000
    name = 'ZhangSan'
    # ---------------------------------------------------------

    zip_name = f'{id}_{name}.zip'
    current_file_directory_path = os.path.dirname(os.path.abspath(__file__))
    input_path = current_file_directory_path
    output_path = os.path.join(current_file_directory_path, zip_name)
 
    zipDir(input_path, output_path)