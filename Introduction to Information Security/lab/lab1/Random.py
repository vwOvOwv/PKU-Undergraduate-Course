# -*- coding: utf-8 -*-
from Crypto.Hash import MD5, SHA1, SHA256
from binascii import hexlify
import os
import platform


def cls():
    os.system('cls' if platform.system() == 'Windows' else 'clear')


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
    buckets = [0] * 8

    while count < 3:
        count += 1
        state = next_random(hash_algo, state)
        buckets[state[0] & 7] += 1
        print('A[%d] = %-30s = %s' % (
            count,
            (hash_name + '(') * count + seed.decode() + ')' * count,
            hexlify(state).decode('ascii')
        ))

    input('按回车继续')

    while True:
        count += 1
        state = next_random(hash_algo, state)
        buckets[state[0] & 7] += 1

        if count % 10000 == 0:
            cls()
            print('Counts = %d' % count)
            mx = max(buckets)
            for i in range(8):
                print('Bucket %d: %-25s, Hits=%d' % (
                    i,
                    '*' * int(buckets[i] / mx * 25),
                    buckets[i]
                ))


if __name__ == '__main__':
    main()
