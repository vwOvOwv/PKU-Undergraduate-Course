import socket

def start_client():
    # 创建套接字，这里的AF_INET和SOCK_STREAM无需深究具体含义
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # 连接到服务端
    client_socket.connect(('localhost', 9999))

    while True:
        # 从用户输入获取信息
        message = input("输入信息(键入'exit'以退出): ")
        if message.lower() == 'exit':
            break
        
        # 发送信息给服务端
        client_socket.sendall(message.encode('utf-8'))

        # 获取服务端发送的信息并打印
        response = client_socket.recv(1024).decode('utf-8')
        print(f"从服务端接收到的信息: {response}")

    print("断开连接")
    client_socket.close()

if __name__ == "__main__":
    start_client()
