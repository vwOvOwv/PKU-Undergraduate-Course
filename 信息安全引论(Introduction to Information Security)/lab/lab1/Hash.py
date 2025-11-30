# -*- coding: utf-8 -*-
from pathlib import Path
from Crypto.Hash import MD5, SHA1, SHA256
from binascii import hexlify


def main():
    mode = input('[1] MD5 \n[2] SHA1 \n[3] SHA256\n请选择模式: ')
    if mode not in ('1', '2', '3'):
        print('输入不合法，退出')
        return

    input_type = input('[1] 字符串 \n[2] 文件 \n请选择数据输入方式: ')
    if input_type not in ('1', '2'):
        print('输入不合法，退出')
        return

    if input_type == '1':
        content = input('请输入数据: ').encode()
    elif input_type == '2':
        filename = input('请输入文件名（直接回车默认为 hello.txt）: ')
        if filename == '':
            filename = 'hello.txt'
        if not Path(filename).exists():
            print('文件 %s 不存在，退出' % filename)
        with open(filename, 'rb') as fd:
            content = fd.read()

    if mode == '1':
        hash_name = 'MD5'
        hash_algo = MD5
    elif mode == '2':
        hash_name = 'SHA1'
        hash_algo = SHA1
    elif mode == '3':
        hash_name = 'SHA256'
        hash_algo = SHA256

    h = hash_algo.new()
    h.update(content)
    print('%s(%s) = "%s"' % (
        hash_name,
        content.decode() if input_type == '1' else filename,
        hexlify(h.digest()).decode('ascii')
    ))


if __name__ == '__main__':
    main()
