# -*- coding: utf-8 -*-
from Crypto.Hash import MD5, SHA1, SHA256
from datetime import datetime
from binascii import hexlify


def count_leading_zeros(bytes_obj):
    binary_str = ''.join(format(byte, '08b') for byte in bytes_obj)
    leading_zeros = len(binary_str) - len(binary_str.lstrip('0'))
    return leading_zeros


def next_random(hash_algo, state):
    h = hash_algo.new()
    h.update(state)
    state = h.digest()
    return state


def main():
    mode = input('[1] MD5 \n[2] SHA1 \n[3] SHA256\n请选择模式: ')
    if mode not in ('1', '2', '3'):
        print('输入不合法，退出')
        return

    seed = input('请输入随机数种子: ').encode()

    input('按回车开始')

    if mode == '1':
        hash_name = 'MD5'
        hash_algo = MD5
    elif mode == '2':
        hash_name = 'SHA1'
        hash_algo = SHA1
    elif mode == '3':
        hash_name = 'SHA256'
        hash_algo = SHA256

    count = 0
    state = seed
    begin_time = datetime.now()
    last_zeros = 0
    while True:
        count += 1
        backup_state = state
        state = next_random(hash_algo, state)
        leading_zeros = count_leading_zeros(state)
        if leading_zeros > last_zeros:
            print('找到 %3d 个前导零, %s(%s) = %s' % (
                leading_zeros,
                hash_name,
                hexlify(backup_state).decode(),
                hexlify(state).decode()
            ))
            print('花费时间: %.2f 秒' % (datetime.now() - begin_time).total_seconds())
            last_zeros = leading_zeros


if __name__ == '__main__':
    main()
