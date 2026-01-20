import os
import sys
sys.path.insert(0, os.path.abspath('.'))
import argparse
import json

from collections import defaultdict

from utils.eval_utils import score_generation, \
    score_generation_by_type, \
    coco_gen_format_save

parser = argparse.ArgumentParser()
parser.add_argument('--results_dir', default='/home/data10T/lpy/mml-hw3/20251223_031922/test_output/captions')
parser.add_argument('--anno', default= '/home/data10T/lpy/clevr-change-dataset/total_change_captions_reformat_subset.json')
parser.add_argument('--type_file', default='/home/data10T/lpy/clevr-change-dataset/type_mapping.json')
args = parser.parse_args()

results = os.listdir(args.results_dir)
results_path = os.path.join(args.results_dir, 'eval_results.txt')
# if os.path.exists(results_path):
#     raise Exception('Result file already exists!')

total_best_results = defaultdict(lambda : ('iter', -10000))
sc_best_results = defaultdict(lambda : ('iter', -10000))
nsc_best_results = defaultdict(lambda : ('iter', -10000))

f = open(results_path, 'w')
for res in results:
    path = os.path.join(args.results_dir, res)
    sc_path = os.path.join(path, 'sc_results.json')
    sc_eval_result = score_generation(args.anno, sc_path)
    sc_eval_result_by_type = score_generation_by_type(args.anno, sc_path, args.type_file)
    sc_captions = json.load(open(sc_path, 'r'))
    message = '===================={} results===================\n'.format(res)
    message += '-------------semantic change captions only----------\n'
    for k, v in sc_eval_result.items():
        iter_name , prev_best = sc_best_results[k]
        if prev_best < v:
            sc_best_results[k] = (res, v)
        message += '{}: {}\n'.format(k, v)
    message += '-------------semantic change captions only (BY TYPE)----------\n'
    for type, eval in sc_eval_result_by_type.items():
        message += '[{}]\n'.format(type.upper())
        for k, v in eval.items():
            message += '{}: {}\n'.format(k, v)
    f.write(message)

summary_message = '\n\n\n=========Results Summary==========\n'
summary_message += '------------semantic change best result-------------\n'
for metric, pairs in sc_best_results.items():
    summary_message += '{}: {} ({})\n'.format(metric, pairs[1], pairs[0])
f.write(summary_message)
f.close()