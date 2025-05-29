import os
import zipfile


def zipHW4(input_path: str, output_path: str, zip_name: str):
    zip = zipfile.ZipFile(output_path, "w", zipfile.ZIP_DEFLATED)

    result_files = {
        'PointNet':['model.py'] + [f'results/{i}.png' for i in range(4)] + ['results/classification_256.png', 'results/classification_1024.png', 'results/segmentation.png'],
        'MaskRCNN':['dataset.py'] + [f'results/{i}_data.png' for i in range(4)] + [f'results/{i % 4}_result_{i // 4}.png' for i in range(16)],
        'RNN':['rnn.py', 'rnn_layers.py'] + [f'results/pred_{split}_{i}.png' for i in range(2) for split in ['train', 'val']] + ['results/rnn_loss_history.png', 'results/single_rnn_layer.npy', 'results/rnn.npy']
    }

    for folder, file_names in result_files.items():
        for file_name in file_names:
            if os.path.exists(os.path.join(input_path, folder, file_name)):
                zip.write(os.path.join(input_path, folder, file_name), os.path.join(f'HW4_{zip_name}', folder, file_name))
            else:
                print(f"File {file_name} not found in {folder}")
                raise FileNotFoundError
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
 
    zipHW4(input_path, output_path, zip_name.split(".")[0])
