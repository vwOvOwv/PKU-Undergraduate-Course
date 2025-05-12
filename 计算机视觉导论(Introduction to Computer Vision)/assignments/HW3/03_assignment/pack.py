import os
import zipfile


def zipHW3(input_path: str, output_path: str, zip_name: str):
    zip = zipfile.ZipFile(output_path, "w", zipfile.ZIP_DEFLATED)
    for path, dirnames, filenames in os.walk(os.path.join(input_path, "camera_calibr")):
        fpath = path.replace(input_path, f'HW3_{zip_name}')
        for filename in filenames:
            if filename in ["calibr.ipynb"]:
                zip.write(os.path.join(path, filename), os.path.join(fpath, filename))
    for path, dirnames, filenames in os.walk(os.path.join(input_path, "depth_pc")):
        fpath = path.replace(input_path, f'HW3_{zip_name}')
        for filename in filenames:
            if filename in ["depth_pc.ipynb"]:
                zip.write(os.path.join(path, filename), os.path.join(fpath, filename))
    for path, dirnames, filenames in os.walk(os.path.join(input_path, "marching_cube")):
        fpath = path.replace(input_path, f'HW3_{zip_name}')
        for filename in filenames:
            if filename in ["marching_cube.ipynb"]:
                zip.write(os.path.join(path, filename), os.path.join(fpath, filename))
    for path, dirnames, filenames in os.walk(os.path.join(input_path, "mesh_pc")):
        fpath = path.replace(input_path, f'HW3_{zip_name}')
        for filename in filenames:
            if filename in ["mesh_pc.ipynb"]:
                zip.write(os.path.join(path, filename), os.path.join(fpath, filename))
    for path, dirnames, filenames in os.walk(os.path.join(input_path, "results")):
        fpath = path.replace(input_path, f'HW3_{zip_name}')
        for filename in filenames:
            zip.write(os.path.join(path, filename), os.path.join(fpath, filename))
    zip.close()


if __name__ == "__main__":
    
    # ---------------------------------------------------------
    # 请用你的学号和姓名替换下面的内容，注意参照例子的格式，使用拼音而非中文
    id = 1900012950
    name = 'ZhangSan'
    # ---------------------------------------------------------

    zip_name = f'{id}_{name}.zip'
    input_path = os.path.dirname(os.path.abspath(__file__))
    output_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), zip_name)
 
    zipHW3(input_path, output_path, zip_name.split(".")[0])
