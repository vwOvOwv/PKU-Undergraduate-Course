import socket
import multiprocessing
import time
import random

def sign_up(process_id, testcase):

    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(('localhost', 9999))

    for tc in testcase:
        time.sleep(random.random() * 2 + 1)   # sleep for 1~3s

        message = ','.join(tc)
        client.send(message.encode('utf-8'))

        response = client.recv(1024).decode('utf-8')
        print(f"Process {process_id} received: {response}")
        if response == 'ok':
            break
    client.close()

testcases_ = [
    [
        ('Alice', 'This1sAlice', '12345678901', 'alice.hi@test.com'),
        (' ', ' ', ' ', ' ')
    ],
    [
        ('Bob', 'Bob1sVeryNB', 'notDigit12345678901', 'bob-nb@test.com'),
        ('Bob', 'No1inBobPhone', '23456789023', 'bob-nb@test.com'),
        (' ', ' ', ' ', ' ')
    ],
    [
        ('Carol', 'Car01PlayGensh1n', '10101010101', 'carol@tsinghua.edu.cn'),
        ('Carol', 'Car01PlayGensh1n', '10101010101', 'carol@test.com '),
        ('Carol', 'Car01Uninsta11Gensh1n', '10101010101', 'carol@test.com'),
        (' ', ' ', ' ', ' ')
    ],
]

if __name__ == "__main__":

    # 预处理testcase
    testcases = []
    for i in range(10):
        for testcase_ in testcases_:
            testcase = []
            suffix = str(random.randint(0, 3))
            for tc in testcase_:
                testcase.append((
                    tc[0] + (suffix if tc[0] != ' ' else ''), *tc[1:]))
            testcases.append(testcase)

    processes = []
    for i in range(30):
        process = multiprocessing.Process(
            target=sign_up, 
            args=(i, testcases[i])
        )
        processes.append(process)
        process.start()

    for process in processes:
        process.join()

    print("All processes have finished.")
