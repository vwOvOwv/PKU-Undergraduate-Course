import sys
import os
sys.path.insert(0, os.path.abspath('.'))
import json
import argparse
import random
import h5py
import numpy as np

from collections import defaultdict
from utils.preprocess import tokenize, encode, build_vocab

parser = argparse.ArgumentParser()
parser.add_argument('--input_captions_json', default='/home/data10T/lpy/clevr-change-dataset/change_captions.json')
parser.add_argument('--input_neg_captions_json', default = '/home/data10T/lpy/clevr-change-dataset/no_change_captions.json')
parser.add_argument('--input_image_dir', default = '/home/data10T/lpy/clevr-change-dataset/images')
parser.add_argument('--input_vocab_json', default='')
parser.add_argument('--split_json', default = '/home/data10T/lpy/clevr-change-dataset/splits.json')
parser.add_argument('--output_vocab_json', default='/home/data10T/lpy/clevr-change-dataset/transformer_vocab.json')
parser.add_argument('--output_h5',default='/home/data10T/lpy/clevr-change-dataset/transformer_labels.h5')

parser.add_argument('--word_count_threshold', default=1, type=int)
parser.add_argument('--allow_unk', default=0, type=int)

def main(args):
    print('Loading captions')
    with open(args.input_captions_json, 'r') as f:
        captions = json.load(f)
    with open(args.input_neg_captions_json, 'r') as f:
        neg_captions = json.load(f)
    with open(args.split_json, 'r') as f:
        splits = json.load(f)
    all_imgs = sorted(os.listdir(args.input_image_dir))
    captioned_imgs = list(captions.keys())
    all_captions = []
    for img, caps in captions.items():
        all_captions.extend(caps)
    all_neg_captions = []
    for img, caps in neg_captions.items():
        all_neg_captions.extend(caps)

    # Extract train data points
    train_split = splits['train']
    train_imgs = [all_imgs[idx] for idx in train_split]
    train_captions = []
    train_neg_captions = []
    for img in train_imgs:
        cap = captions[img]
        neg_cap = neg_captions[img]
        train_captions.extend(cap)
        train_neg_captions.extend(neg_cap)

    N = len(all_imgs)
    N_captioned = len(captions)
    M = len(all_captions)
    M_neg = len(all_neg_captions)
    print('Total images: %d' % N)
    print('Total captioned images: %d' % N_captioned)
    print('Total captions: %d' % M)
    print('Total negative captions: %d' % M_neg)
    print('Total train images: %d' % len(train_imgs))
    print('Total train captions: %d' % len(train_captions))
    print('Total train neg captions: %d' % len(train_neg_captions))

    # Either create the vocab or load it from disk
    if args.input_vocab_json == '':
        print('Building vocab')
        word_to_idx = build_vocab(
            train_captions + train_neg_captions,
            min_token_count=args.word_count_threshold,
            punct_to_keep=[';', ','], punct_to_remove=['?', '.']
        )
    else:
        print('Loading vocab')
        with open(args.input_vocab_json, 'r') as f:
            word_to_idx = json.load(f)
    if args.output_vocab_json != '':
        with open(args.output_vocab_json, 'w') as f:
            json.dump(word_to_idx, f)

    # Encode all captions
    # First, figure out max length of captions
    all_cap_tokens = []
    max_length = -1
    cap_keys = sorted(list(captions.keys()))
    for img in cap_keys:
        caps = captions[img]
        n = len(caps)
        assert n > 0, 'error: some image has no caption'
        tokens_list = []
        for cap in caps:
            cap_tokens = tokenize(cap,
                                  add_start_token=True,
                                  add_end_token=True,
                                  punct_to_keep=[';', ','],
                                  punct_to_remove=['?', '.'])
            tokens_list.append(cap_tokens)
            max_length = max(max_length, len(cap_tokens))
        all_cap_tokens.append((img, tokens_list))

    all_neg_cap_tokens = []
    cap_keys = sorted(list(captions.keys()))
    for img in cap_keys:
        neg_caps = neg_captions[img]
        neg_n = len(neg_caps)
        assert neg_n > 0, 'error: some image has no caption'
        neg_tokens_list = []
        for neg_cap in neg_caps:
            neg_cap_tokens = tokenize(neg_cap,
                                      add_start_token=True,
                                      add_end_token=True,
                                      punct_to_keep=[';', ','],
                                      punct_to_remove=['?', '.'])
            neg_tokens_list.append(neg_cap_tokens)
        all_neg_cap_tokens.append((img, neg_tokens_list))

    print('Encoding captions')
    label_arrays = []
    label_start_idx = -np.ones(N, dtype=np.int64)
    label_end_idx = -np.ones(N, dtype=np.int64)
    label_length = np.zeros(M, dtype=np.int64)
    caption_counter = 0
    counter = 0

    # Then encode
    for img, tokens_list in all_cap_tokens:
        i = int(img.split('.')[0].split('_')[-1])
        n = len(tokens_list)
        Li = np.zeros((n, max_length), dtype=np.int64)
        for j, tokens in enumerate(tokens_list):
            label_length[caption_counter] = len(tokens)
            caption_counter += 1
            tokens_encoded = encode(tokens,
                                    word_to_idx,
                                    allow_unk=args.allow_unk == 1)
            for k, w in enumerate(tokens_encoded):
                Li[j, k] = w
        # captions are padded with zeros
        label_arrays.append(Li)
        label_start_idx[i] = counter
        label_end_idx[i] = counter + n - 1

        counter += n

    L = np.concatenate(label_arrays, axis=0) # put all labels together
    assert L.shape[0] == M, "lengths don't match?"
    assert np.all(label_length > 0), 'error: some captions have no word?'

    print('Encoding negative captions')
    neg_label_arrays = []
    neg_label_start_idx = -np.ones(N, dtype=np.int64)
    neg_label_end_idx = -np.ones(N, dtype=np.int64)
    neg_label_length = np.zeros(M_neg, dtype=np.int64)
    neg_caption_counter = 0
    neg_counter = 0

    # Then encode
    for img, tokens_list in all_neg_cap_tokens:
        i = int(img.split('.')[0].split('_')[-1])
        n = len(tokens_list)
        Li = np.zeros((n, max_length), dtype=np.int64)
        for j, tokens in enumerate(tokens_list):
            neg_label_length[neg_caption_counter] = len(tokens)
            neg_caption_counter += 1
            tokens_encoded = encode(tokens,
                                    word_to_idx,
                                    allow_unk=args.allow_unk == 1)
            for k, w in enumerate(tokens_encoded):
                Li[j, k] = w
        # captions are padded with zeros
        neg_label_arrays.append(Li)
        neg_label_start_idx[i] = neg_counter
        neg_label_end_idx[i] = neg_counter + n - 1

        neg_counter += n

    neg_L = np.concatenate(neg_label_arrays, axis=0) # put all labels together
    assert neg_L.shape[0] == M_neg, "lengths don't match?"
    assert np.all(neg_label_length > 0), 'error: some captions have no word?'

    # Create h5 file
    print('Writing output')
    print('Encoded captions array size: ', L.shape)
    print('Encoded negative captions array size: ', neg_L.shape)
    with h5py.File(args.output_h5, 'w') as f:
        f.create_dataset('labels', data=L)
        f.create_dataset('label_start_idx', data=label_start_idx)
        f.create_dataset('label_end_idx', data=label_end_idx)
        f.create_dataset('label_length', data=label_length)
        f.create_dataset('neg_labels', data=neg_L)
        f.create_dataset('neg_label_start_idx', data=neg_label_start_idx)
        f.create_dataset('neg_label_end_idx', data=neg_label_end_idx)
        f.create_dataset('neg_label_length', data=neg_label_length)

if __name__ == '__main__':
    args = parser.parse_args()
    main(args)
