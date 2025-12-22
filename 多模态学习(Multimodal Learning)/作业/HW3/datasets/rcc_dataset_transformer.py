import os
import json
import numpy as np
import random
import time

import h5py
import torch
from torch.utils.data import Dataset, DataLoader
from torch.utils.data.dataloader import default_collate


class RCCDataset(Dataset):
    shapes = set(['ball', 'block', 'cube', 'cylinder', 'sphere'])
    sphere = set(['ball', 'sphere'])
    cube = set(['block', 'cube'])
    cylinder = set(['cylinder'])

    colors = set(['red', 'cyan', 'brown', 'blue', 'purple', 'green', 'gray', 'yellow'])

    materials = set(['metallic', 'matte', 'rubber', 'shiny', 'metal'])
    rubber = set(['matte', 'rubber'])
    metal = set(['metal', 'metallic', 'shiny'])

    type_to_label = {
        'color': 0,
        'material': 1,
        'add': 2,
        'drop': 3,
        'move': 4,
        'no_change': 5
    }

    def __init__(self, cfg, split):
        self.cfg = cfg

        print('Speaker Dataset loading vocab json file: ', cfg.data.vocab_json)
        self.vocab_json = cfg.data.vocab_json
        self.word_to_idx = json.load(open(self.vocab_json, 'r'))
        self.idx_to_word = {}
        for word, idx in self.word_to_idx.items():
            self.idx_to_word[idx] = word
        self.vocab_size = len(self.idx_to_word)
        print('vocab size is ', self.vocab_size)

        self.type_mapping = json.load(open(cfg.data.type_mapping_json, 'r'))
        self.type_to_img = {}
        for k, v in self.type_mapping.items():
            self.type_to_img[k] = set([int(x.split('.')[0]) for x in v])

        self.d_feat_dir = cfg.data.default_feature_dir
        self.s_feat_dir = cfg.data.semantic_feature_dir
        self.n_feat_dir = cfg.data.nonsemantic_feature_dir

        self.d_feats = sorted(os.listdir(self.d_feat_dir))
        self.s_feats = sorted(os.listdir(self.s_feat_dir))
        self.n_feats = sorted(os.listdir(self.n_feat_dir))

        assert len(self.d_feats) == len(self.s_feats) == len(self.n_feats), \
            'The number of features are different from each other!'

        self.d_img_dir = cfg.data.default_img_dir
        self.s_img_dir = cfg.data.semantic_img_dir
        self.n_img_dir = cfg.data.nonsemantic_img_dir

        self.d_imgs = sorted(os.listdir(self.d_img_dir))
        self.s_imgs = sorted(os.listdir(self.s_img_dir))
        self.n_imgs = sorted(os.listdir(self.n_img_dir))

        self.splits = json.load(open(cfg.data.splits_json, 'r'))
        self.split = split

        if split == 'train':
            self.batch_size = cfg.data.train.batch_size
            self.seq_per_img = cfg.data.train.seq_per_img
            self.split_idxs = self.splits['train']
            self.num_samples = len(self.split_idxs)
            if cfg.data.train.max_samples is not None:
                self.num_samples = min(cfg.data.train.max_samples, self.num_samples)
        elif split == 'val':
            self.batch_size = cfg.data.val.batch_size
            self.seq_per_img = cfg.data.val.seq_per_img
            self.split_idxs = self.splits['val']
            self.num_samples = len(self.split_idxs)
            if cfg.data.val.max_samples is not None:
                self.num_samples = min(cfg.data.val.max_samples, self.num_samples)
        elif split == 'test':
            self.batch_size = cfg.data.test.batch_size
            self.seq_per_img = cfg.data.test.seq_per_img
            self.split_idxs = self.splits['test']
            self.num_samples = len(self.split_idxs)
            if cfg.data.test.max_samples is not None:
                self.num_samples = min(max_samples, self.num_samples)
        else:
            raise Exception('Unknown data split %s' % split)

        print("Dataset size for %s: %d" % (split, self.num_samples))

        # load in the sequence data
        self.h5_label_file = h5py.File(cfg.data.h5_label_file, 'r')
        seq_size = self.h5_label_file['labels'].shape
        self.labels = self.h5_label_file['labels'][:]  # just gonna load...
        self.neg_labels = self.h5_label_file['neg_labels'][:]
        self.max_seq_length = seq_size[1]
        self.IGNORE = -1
        self.label_start_idx = self.h5_label_file['label_start_idx'][:]
        self.label_end_idx = self.h5_label_file['label_end_idx'][:]
        self.neg_label_start_idx = self.h5_label_file['neg_label_start_idx'][:]
        self.neg_label_end_idx = self.h5_label_file['neg_label_end_idx'][:]
        print('Max sequence length is %d' % self.max_seq_length)
        self.h5_label_file.close()

    def __len__(self):
        return self.num_samples

    def __getitem__(self, index):
        # random.seed(1111)
        img_idx = self.split_idxs[index]

        # Fetch image data
        # one easy way to augment data is to use nonsemantically changed
        # scene as the default :)
        if self.split == 'train':
            if random.random() < 0.5:
                d_feat_path = os.path.join(self.d_feat_dir, self.d_feats[img_idx])
                d_img_path = os.path.join(self.d_img_dir, self.d_imgs[img_idx])
                n_feat_path = os.path.join(self.n_feat_dir, self.n_feats[img_idx])
                n_img_path = os.path.join(self.n_img_dir, self.n_imgs[img_idx])
            else:
                d_feat_path = os.path.join(self.n_feat_dir, self.n_feats[img_idx])
                d_img_path = os.path.join(self.n_img_dir, self.n_imgs[img_idx])
                n_feat_path = os.path.join(self.d_feat_dir, self.d_feats[img_idx])
                n_img_path = os.path.join(self.d_img_dir, self.d_imgs[img_idx])
        else:
            d_feat_path = os.path.join(self.d_feat_dir, self.d_feats[img_idx])
            d_img_path = os.path.join(self.d_img_dir, self.d_imgs[img_idx])
            n_feat_path = os.path.join(self.n_feat_dir, self.n_feats[img_idx])
            n_img_path = os.path.join(self.n_img_dir, self.n_imgs[img_idx])

        q_feat_path = os.path.join(self.s_feat_dir, self.s_feats[img_idx])
        q_img_path = os.path.join(self.s_img_dir, self.s_imgs[img_idx])

        d_feature = torch.FloatTensor(np.load(d_feat_path))
        n_feature = torch.FloatTensor(np.load(n_feat_path))
        q_feature = torch.FloatTensor(np.load(q_feat_path))

        # Fetch change type labels
        aux_label_pos = -1
        for type, img_set in self.type_to_img.items():
            if img_idx in img_set:
                aux_label_pos = self.type_to_label[type]
                break
        aux_label_neg = self.type_to_label['no_change']

        # Fetch sequence labels
        ix1 = self.label_start_idx[img_idx]
        ix2 = self.label_end_idx[img_idx]
        n_cap = ix2 - ix1 + 1

        seq = np.zeros([self.seq_per_img, self.max_seq_length], dtype=int)
        if n_cap < self.seq_per_img:
            # we need to subsample (with replacement)
            for q in range(self.seq_per_img):
                ixl = random.randint(ix1, ix2)
                seq[q, :self.max_seq_length] = \
                    self.labels[ixl, :self.max_seq_length]
        else:
            ixl = random.randint(ix1, ix2 - self.seq_per_img + 1)
            seq[:, :self.max_seq_length] = \
                self.labels[ixl: ixl + self.seq_per_img, :self.max_seq_length]

        # Fetch negative sequence labels
        ix1 = self.neg_label_start_idx[img_idx]
        ix2 = self.neg_label_end_idx[img_idx]
        n_cap = ix2 - ix1 + 1

        neg_seq = np.zeros([self.seq_per_img, self.max_seq_length], dtype=int)
        if n_cap < self.seq_per_img:
            # we need to subsample (with replacement)
            for q in range(self.seq_per_img):
                ixl = random.randint(ix1, ix2)
                neg_seq[q, :self.max_seq_length] = \
                    self.neg_labels[ixl, :self.max_seq_length]
        else:
            ixl = random.randint(ix1, ix2 - self.seq_per_img + 1)
            neg_seq[:, :self.max_seq_length] = \
                self.neg_labels[ixl: ixl + self.seq_per_img, :self.max_seq_length]

        # Generate masks
        mask = np.zeros_like(seq)
        nonzeros = np.array(list(map(lambda x: (x != 0).sum(), seq)))
        for ix, row in enumerate(mask):
            row[:nonzeros[ix]] = 1
        if seq.size == self.max_seq_length:
            labels_with_ignore_tolish = [self.IGNORE if m == 0 else tid
                      for tid, m in zip(seq.squeeze(0).tolist(), mask.squeeze(0).tolist())][1:] + [self.IGNORE]
            labels_with_ignore = np.array(labels_with_ignore_tolish)
            labels_with_ignore = np.expand_dims(labels_with_ignore, 0)
        else:
            labels_with_ignore = np.zeros_like(neg_seq)

        neg_mask = np.zeros_like(neg_seq)
        nonzeros = np.array(list(map(lambda x: (x != 0).sum(), neg_seq)))
        for ix, row in enumerate(neg_mask):
            row[:nonzeros[ix]] = 1
        if neg_seq.size == self.max_seq_length:
            neg_labels_with_ignore_tolish = [self.IGNORE if m == 0 else tid
                                     for tid, m in zip(neg_seq.squeeze(0).tolist(), neg_mask.squeeze(0).tolist())][1:] + [
                                        self.IGNORE]
            neg_labels_with_ignore = np.array(neg_labels_with_ignore_tolish)
            neg_labels_with_ignore = np.expand_dims(neg_labels_with_ignore, 0)
        else:
            neg_labels_with_ignore = np.zeros_like(neg_seq)

        return (d_feature, n_feature, q_feature,
                seq, labels_with_ignore, neg_seq, neg_labels_with_ignore, mask, neg_mask, aux_label_pos, aux_label_neg,
                d_img_path, n_img_path, q_img_path)

    def get_vocab_size(self):
        return self.vocab_size

    def get_idx_to_word(self):
        return self.idx_to_word

    def get_word_to_idx(self):
        return self.word_to_idx

    def get_max_seq_length(self):
        return self.max_seq_length


def rcc_collate(batch):
    transposed = list(zip(*batch))
    d_feat_batch = transposed[0]
    n_feat_batch = transposed[1]
    q_feat_batch = transposed[2]
    seq_batch = default_collate(transposed[3])
    label_with_ignore_batch = default_collate(transposed[4])
    neg_seq_batch = default_collate(transposed[5])
    neg_label_with_ignore_batch = default_collate(transposed[6])
    mask_batch = default_collate(transposed[7])
    neg_mask_batch = default_collate(transposed[8])
    aux_label_pos_batch = default_collate(transposed[9])
    aux_label_neg_batch = default_collate(transposed[10])
    if any(f is not None for f in d_feat_batch):
        d_feat_batch = default_collate(d_feat_batch)
    if any(f is not None for f in n_feat_batch):
        n_feat_batch = default_collate(n_feat_batch)
    if any(f is not None for f in q_feat_batch):
        q_feat_batch = default_collate(q_feat_batch)

    d_img_batch = transposed[11]
    n_img_batch = transposed[12]
    q_img_batch = transposed[13]
    return (d_feat_batch, n_feat_batch, q_feat_batch,
            seq_batch, label_with_ignore_batch, neg_seq_batch, neg_label_with_ignore_batch,
            mask_batch, neg_mask_batch,
            aux_label_pos_batch, aux_label_neg_batch,
            d_img_batch, n_img_batch, q_img_batch)


class RCCDataLoader(DataLoader):

    def __init__(self, dataset, **kwargs):
        kwargs['collate_fn'] = rcc_collate
        super().__init__(dataset, **kwargs)
