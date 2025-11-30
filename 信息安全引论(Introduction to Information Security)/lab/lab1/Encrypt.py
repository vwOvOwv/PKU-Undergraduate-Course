# -*- coding: utf-8 -*-
import base64
from Crypto.Cipher import AES


class EncryptDataCBC:
    def __init__(self, key, iv):
        self.key = key.encode("utf-8")                          # 初始化密钥
        self.iv = iv.encode("utf-8")                            # 初始化偏移量
        self.length = 16                                        # 初始化数据块大小
        self.aes = AES.new(self.key, AES.MODE_CBC,
                           self.iv)     # 初始化AES,ECB模式的实例
        # 截断函数，去除填充的字符
        self.unpad = lambda s: s[0:-s[-1]]

    def pad(self, text):
        """
        填充函数, 使被加密数据的字节码长度是block_size的整数倍
        """
        count = len(text.encode('utf-8'))
        add = self.length - (count % self.length)
        entext = text + (chr(add) * add)
        return entext

    def encrypt(self, encrData):  # 加密函数
        a = self.pad(encrData)
        res = self.aes.encrypt(a.encode("utf-8"))
        msg = str(base64.b64encode(res), encoding="utf8")
        return msg

    def decrypt(self, decrData):  # 解密函数
        res = base64.decodebytes(decrData.encode("utf-8"))
        msg_text = self.aes.decrypt(res)
        decrypt_text = self.unpad(msg_text).decode('utf8', errors='ignore')
        return decrypt_text


class EncryptDataECB:
    def __init__(self, key):
        self.key = key.encode("utf-8")                          # 初始化密钥
        self.length = 16                                        # 初始化数据块大小
        # 初始化AES,ECB模式的实例
        self.aes = AES.new(self.key, AES.MODE_ECB)
        # 截断函数，去除填充的字符
        self.unpad = lambda s: s[0:-s[-1]]

    def pad(self, text):
        """
        填充函数, 使被加密数据的字节码长度是block_size的整数倍
        """
        count = len(text.encode('utf-8'))
        add = self.length - (count % self.length)
        entext = text + (chr(add) * add)
        return entext

    def encrypt(self, encrData):  # 加密函数
        a = self.pad(encrData)
        res = self.aes.encrypt(a.encode("utf-8"))
        msg = str(base64.b64encode(res), encoding="utf8")
        return msg

    def decrypt(self, decrData):  # 解密函数
        res = base64.decodebytes(decrData.encode("utf-8"))
        msg_text = self.aes.decrypt(res)
        decrypt_text = self.unpad(msg_text).decode('utf8', errors='ignore')
        return decrypt_text


if __name__ == '__main__':
    aes_key = "0CoJUm6Qyw8W8jud"
    aes_iv = "9999999999999999"
    mode = input("[1] AES-ECB\n[2] AES-CBC\n请选择模式: ")
    work = input("[1] Encrypt\n[2] Decrypt\n请选择模式: ")

    input_file = open("input.txt", "r", encoding='utf-8')
    text_data = input_file.read()
    input_file.close()

    if mode == "1" and work == "1":
        output_data = EncryptDataECB(aes_key).encrypt(text_data)
    elif mode == "2" and work == "1":
        output_data = EncryptDataCBC(aes_key, aes_iv).encrypt(text_data)
    elif mode == "1" and work == "2":
        output_data = EncryptDataECB(aes_key).decrypt(text_data)
    elif mode == "2" and work == "2":
        output_data = EncryptDataCBC(aes_key, aes_iv).decrypt(text_data)

    output_file = open("output.txt", "w", encoding='utf-8')
    output_file.write(output_data)
    output_file.close()
