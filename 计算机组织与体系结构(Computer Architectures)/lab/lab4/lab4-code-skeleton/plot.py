import numpy as np
import matplotlib.pyplot as plt
import argparse

args = argparse.ArgumentParser()
args.add_argument("--file", type=str, default="latency")

args = args.parse_args()

csv_file = args.file + ".csv"
png_file = args.file + ".png"

with open(csv_file, "r") as f:
    lines = f.readlines()
    lines = [line.strip() for line in lines]
    lines = [line.split(",") for line in lines]
    lines = [[float(x) for x in line] for line in lines]
    lines = np.array(lines)
    lines = lines.T

    fig = plt.figure(figsize=(8, 4))
    ax = fig.add_subplot(111)
    # ax.set_xlabel("Latency (ms)")
    # ax.set_ylabel("CDF (%)")
    ax.set_xlabel("Request (th)", fontsize=12)
    ax.set_ylabel("Latency (ms)", fontsize=12)
    
    sorted_data = np.sort(np.divide(lines[3], 1000000000))
    cdf = np.arange(1, len(sorted_data)+1) / len(sorted_data)
    cdf = np.multiply(cdf, 100)
    
    ax.set_title("Latency over progress")
    ax.scatter(lines[0], np.divide(lines[3], 1000000000), color="red", label="", s=3)
    # ax.set_ylim(0, 10)
    ax.legend(frameon=False)
    plt.savefig(png_file, dpi=500)
    plt.show()
