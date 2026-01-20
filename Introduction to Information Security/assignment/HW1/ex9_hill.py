import numpy as np

def get_modular_inverse(a, m):
    """Compute (1 / a % m) % m"""
    a = a % m
    try:
        inv = pow(a, -1, m)
        return inv
    except ValueError:
        raise ValueError(f"The determinant {a} has no modular inverse for modulus {m}")

def get_key():
    # get random key
    while True:
        key = np.random.randint(low=0, high=26, size=(2, 2))
        det = int(np.round(np.linalg.det(key)))
        if np.gcd(det, 26) == 1:    # valid key
            return key

def get_inverse_key(key):
    """Compute inverse matrix of key (mod 26)"""
    det = int(np.round(np.linalg.det(key)))
    det_inv = get_modular_inverse(det, 26)
    adjugate_matrix = np.array([[key[1, 1], -key[0, 1]],
                                [-key[1, 0], key[0, 0]]])
    inv_key = (det_inv * adjugate_matrix) % 26
    return inv_key

def hill_cipher(text, key, mode='encrypt'):
    """
    Args:
        text (str): plaintext or ciphertext
        key (np.array): 2x2 key matrix
        mode (str): 'encrypt' or 'decrypt'.

    Returns:
        str: encrypted or decrypted text
    """
    if mode == 'decrypt':
        key_matrix = get_inverse_key(key)
    else:
        key_matrix = key
    # convert string into matrix
    text_matrix = np.array([ord(char) - ord('A') for char in text])
    text_matrix = text_matrix.reshape(-1, 2)
    # perform linear transform
    result_matrix = (key_matrix @ text_matrix) % 26
    # convert matrix into string
    result_text = "".join([chr(int(n) + ord('A')) for n in result_matrix.flatten()])
    return result_text


if __name__ == '__main__':
    K = get_key()
    print(f"Key:\n{K}")
    plain_text = "LOVE"
    print(f"Plaintext: {plain_text}")
    cipher_text = hill_cipher(plain_text, K, mode='encrypt')
    print(f"Ciphertext: {cipher_text}")
    decrypted_text = hill_cipher(cipher_text, K, mode='decrypt')
    print(f"Decrypted text: {decrypted_text}")
    if plain_text == decrypted_text:
        print("Success: decrypted text matches plaintext")
    else:
        print("Error: decrypted text dismatches plaintext")