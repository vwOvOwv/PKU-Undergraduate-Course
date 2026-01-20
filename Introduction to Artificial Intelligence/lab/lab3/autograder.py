import argparse
import subprocess
from util import TimeoutFunction
import numpy as np

PYTHON_PATH = "python3"

parser = argparse.ArgumentParser()
parser.add_argument("--q", choices=["q1", "q2", "q3", "all"], default="all")
parser.add_argument("--q2training", action="store_true")
args = parser.parse_args()

def programcall(cmd: str, timeout: float=600):
    # print(cmd)
    func = TimeoutFunction(subprocess.check_output, timeout)
    ret = func(cmd, shell=True)
    ret = str(ret, encoding="utf-8")
    return ret.splitlines()

def checkGraphDebuggerOut(outputlines):
    for line  in outputlines[1:]:
        grad1, grad2 = map(float, line.split())
        if np.abs(grad1-grad2) > 1e-2:
            return False
    return True

def matchGraphDebuggerOut(outputlines, answerlines):
    for i in range(len(outputlines)-1):
        _, grad2 = map(float, outputlines[i+1].split())
        _, answergrad = map(float, answerlines[i+1].split())
        if np.abs(answergrad-grad2) > 1e-2:
            return False
    return True

def filtertestacc(outputlines):
    ret = list(filter(lambda line: "valid acc" in line, outputlines))
    return float(ret[0].split()[-1])

def scorer(acc, standardacc,  zeroacc=0.1, fullacc=1):
    '''
    stardardacc代表90分时的成绩
    zeroacc是0分线
    fullacc是满分线
    '''
    if acc < standardacc:
        a = (90-0)/(standardacc-zeroacc)
        b = 0 - zeroacc * a
    else:
        a = (100-90)/(fullacc - standardacc)
        b = 100 - fullacc * a
    y = a*acc + b
    return max(min(y, 100), 0)

def q1():
    print("q1")
    output = programcall(f"{PYTHON_PATH} testq1.py")
    acc = filtertestacc(output)
    print(f"valid acc {acc:.4f}")
    return scorer(acc, 0.75, 0.5, 0.78)

def q2():
    print("q2")
    output = programcall(f"{PYTHON_PATH} testq2.py")
    acc = filtertestacc(output)
    print(f"valid acc {acc:.4f}")
    return (scorer(acc, 0.6, 0.5, 0.65))


def q3():
    print("q3")
    output = programcall(f"{PYTHON_PATH} testq3.py")
    acc = filtertestacc(output)
    print(f"valid acc {acc:.4f}")
    return scorer(acc, 0.9, 0.0, 1)

def q2training():
    output = programcall(f"{PYTHON_PATH} FruitModel.py")

if __name__ == "__main__":
    if args.q == "all":
        for q in [q1, q2, q3]:
            print(f"score {q():.0f}")
    else:
        if args.q == "q2" and args.q2training:
            q2training()
        print(f"score {eval(args.q)():.0f}")