#!/usr//bin/python
#
# driver.py - The driver tests the correctness of the student's cache
#     simulator and the correctness and performance of their transpose
#     function. It uses ./test-csim to check the correctness of the
#     simulator and it runs ./test-trans on three different sized
#     matrices (32x32, 64x64, and 60x68) to test the correctness and
#     performance of the transpose function.
#
import subprocess;
import re;
import os;
import sys;
import optparse;

#
# computeMissScore - compute the score depending on the number of
# cache misses
#
def computeMissScore(miss, lower, upper, full_score):
    if miss <= lower:
        return full_score
    if miss >= upper: 
        return 0

    score = (miss - lower) * 1.0 
    range = (upper- lower) * 1.0
    return round((1 - score / range) * full_score, 1)

#
# main - Main function
#
def main():

    # Configure maxscores here
    maxscore= {};
    maxscore['csim'] = 27
    maxscore['transc'] = 1
    maxscore['trans32'] = 8
    maxscore['trans64'] = 8
    maxscore['trans60'] = 10

    # Parse the command line arguments 
    p = optparse.OptionParser()
    p.add_option("-A", action="store_true", dest="autograde", 
                 help="emit autoresult string for Autolab");
    opts, args = p.parse_args()
    autograde = opts.autograde

    # Check the correctness of the cache simulator
    print("Part A: Testing cache simulator")
    print("Running ./test-csim")
    p = subprocess.Popen("./test-csim", 
                         shell=True, stdout=subprocess.PIPE)
    stdout_data = p.communicate()[0].decode('utf-8')
    print(stdout_data)

    # Emit the output from test-csim
    stdout_data = re.split('\n', stdout_data)
    for line in stdout_data:
        if re.match("TEST_CSIM_RESULTS", line):
            resultsim = re.findall(r'(\d+)', line)
        else:
            print("%s" % (line))

    # Check the correctness and performance of the transpose function
    # 32x32 transpose
    print("Part B: Testing transpose function")
    print("Running ./test-trans -M 32 -N 32 -t")
    p = subprocess.Popen("./test-trans -M 32 -N 32 -t | grep TEST_TRANS_RESULTS", 
                         shell=True, stdout=subprocess.PIPE)
    stdout_data = p.communicate()[0].decode('utf-8')
    result32 = re.findall(r'(\d+)', stdout_data)
    
    # 64x64 transpose
    print("Running ./test-trans -M 64 -N 64 -t")
    p = subprocess.Popen("./test-trans -M 64 -N 64 -t | grep TEST_TRANS_RESULTS", 
                         shell=True, stdout=subprocess.PIPE)
    stdout_data = p.communicate()[0].decode('utf-8')
    result64 = re.findall(r'(\d+)', stdout_data)
    
    # 60x68 transpose
    print("Running ./test-trans -M 60 -N 68 -t")
    p = subprocess.Popen("./test-trans -M 60 -N 68 -t | grep TEST_TRANS_RESULTS", 
                         shell=True, stdout=subprocess.PIPE)
    stdout_data = p.communicate()[0].decode('utf-8')
    result60 = re.findall(r'(\d+)', stdout_data)
    
    # Compute the scores for each step
    csim_cscore  = list(map(int, resultsim[0:1]))
    trans_cscore = int(result32[0]) * int(result64[0]) * int(result60[0]);
    miss32 = int(result32[1])
    miss64 = int(result64[1])
    miss60 = int(result60[1])
    trans32_score = computeMissScore(miss32, 300, 600, maxscore['trans32']) * int(result32[0])
    trans64_score = computeMissScore(miss64, 1300, 2000, maxscore['trans64']) * int(result64[0])
    trans60_score = computeMissScore(miss60, 1600, 2600, maxscore['trans60']) * int(result60[0])
    total_score = csim_cscore[0] + trans32_score + trans64_score + trans60_score

    # Summarize the results
    print("\nCache Lab summary:")
    print("%-22s%8s%10s%12s" % ("", "Points", "Max pts", "Misses"))
    print("%-22s%8.1f%10d" % ("Csim correctness", csim_cscore[0], 
                              maxscore['csim']))

    misses = str(miss32)
    if miss32 == 2**31-1 :
        misses = "invalid"
    print("%-22s%8.1f%10d%12s" % ("Trans perf 32x32", trans32_score, 
                                  maxscore['trans32'], misses))

    misses = str(miss64)
    if miss64 == 2**31-1 :
        misses = "invalid"
    print("%-22s%8.1f%10d%12s" % ("Trans perf 64x64", trans64_score, 
                                  maxscore['trans64'], misses))

    misses = str(miss60)
    if miss60 == 2**31-1 :
        misses = "invalid"
    print("%-22s%8.1f%10d%12s" % ("Trans perf 60x68", trans60_score, 
                                  maxscore['trans60'], misses))

    print("%22s%8.1f%10d" % ("Total points", total_score,
                             maxscore['csim'] + 
                             maxscore['trans32'] + 
                             maxscore['trans64'] +
                             maxscore['trans60']))
    
    # Emit autoresult string for Autolab if called with -A option
    if autograde:
        autoresult="{\"scores\": {\"Autograded\": %.1f}, \"scoreboard\": [%.1f, %d, %d, %d, %d]}" % (total_score, total_score, miss32+miss64+miss60, miss32, miss64, miss60)
        print("%s" % autoresult)
    
    
# execute main only if called as a script
if __name__ == "__main__":
    main()

