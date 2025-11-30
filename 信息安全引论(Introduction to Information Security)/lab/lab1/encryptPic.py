from Crypto.Cipher import AES
from Crypto import Random
from Crypto.Util.Padding import pad, unpad

key = Random.new().read(AES.block_size)
iv = Random.new().read(AES.block_size)


mode = AES.MODE_CBC
# mode = AES.MODE_ECB


def Encrypt():
    input_file = open("./pic_original.bmp", "rb")
    input_data = input_file.read()
    input_file.close()

    cipher = AES.new(key, AES.MODE_ECB) if mode == AES.MODE_ECB else AES.new(key, AES.MODE_CBC, iv)
    enc_data = cipher.encrypt(pad(input_data, 16))
    correct_data = bytearray(enc_data)
    correct_data[:54] = input_data[:54]

    enc_file = open("encrypted.bmp", "wb")
    enc_file.write(correct_data)
    enc_file.close()

def Decrypt():
    enc_file2 = open("encrypted.enc", "rb")
    enc_data2 = enc_file2.read()
    enc_file2.close()

    decipher = AES.new(key, AES.MODE_ECB) if mode == AES.MODE_ECB else AES.new(key, AES.MODE_CBC, iv)
    plain_data = unpad(decipher.decrypt(enc_data2))

    output_file = open("output.bmp", "wb")
    output_file.write(plain_data)
    output_file.close()

Encrypt()