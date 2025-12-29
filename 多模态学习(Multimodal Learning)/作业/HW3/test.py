import os
import argparse
import json
import time
import numpy as np
import torch
torch.backends.cudnn.enabled  = True
import torch.nn as nn
import torch.nn.functional as F
import cv2
import matplotlib.pyplot as plt

from configs.config_transformer import cfg, merge_cfg_from_file
from datasets.datasets import create_dataset
from models.model import ChangeDetector, AddSpatialInfo
from models.transformer_decoder import Speaker

from utils.utils import AverageMeter, accuracy, set_mode, load_checkpoint, \
                        decode_sequence, decode_sequence_transformer, coco_gen_format_save
from utils.vis_utils import visualize_att
from tqdm import tqdm

# Load config
parser = argparse.ArgumentParser()
parser.add_argument('--cfg', default='configs/dynamic/transformer_fast.yaml')
parser.add_argument('--visualize', action='store_true')
parser.add_argument('--snapshot', type=int, default=10000)
parser.add_argument('--gpu', type=int, default=-1)
args = parser.parse_args()
merge_cfg_from_file(args.cfg)
# assert cfg.exp_name == os.path.basename(args.cfg).replace('.yaml', '')

# Device configuration
use_cuda = torch.cuda.is_available()
if args.gpu == -1:
    gpu_ids = cfg.gpu_id
else:
    gpu_ids = [args.gpu]
torch.backends.cudnn.enabled  = True
default_gpu_device = gpu_ids[0]
torch.cuda.set_device(default_gpu_device)
device = torch.device("cuda" if use_cuda else "cpu")

# Experiment configuration
exp_dir = cfg.exp_dir
exp_name = "20251223_031922"

output_dir = os.path.join(exp_dir, exp_name)

test_output_dir = os.path.join(output_dir, 'test_output')
if not os.path.exists(test_output_dir):
    os.makedirs(test_output_dir)
caption_output_path = os.path.join(test_output_dir, 'captions', 'test')
if not os.path.exists(caption_output_path):
    os.makedirs(caption_output_path)
att_output_path = os.path.join(test_output_dir, 'attentions', 'test')
if not os.path.exists(att_output_path):
    os.makedirs(att_output_path)

if args.visualize:
    visualize_save_dir = os.path.join(test_output_dir, 'visualizations')
    if not os.path.exists(visualize_save_dir):
        os.makedirs(visualize_save_dir)

snapshot_dir = os.path.join(output_dir, 'snapshots')
snapshot_file = '%s_checkpoint_%d.pt' % (exp_name, args.snapshot)
snapshot_full_path = os.path.join(snapshot_dir, snapshot_file)
checkpoint = load_checkpoint(snapshot_full_path)
change_detector_state = checkpoint['change_detector_state']
speaker_state = checkpoint['speaker_state']


# Load modules
change_detector = ChangeDetector(cfg)
change_detector.load_state_dict(change_detector_state)
change_detector = change_detector.to(device)

speaker = Speaker(cfg)
speaker.load_state_dict(speaker_state)
speaker.to(device)

spatial_info = AddSpatialInfo()
spatial_info.to(device)

print(change_detector)
print(speaker)
print(spatial_info)

# Data loading part
train_dataset, train_loader = create_dataset(cfg, 'train')
idx_to_word = train_dataset.get_idx_to_word()
test_dataset, test_loader = create_dataset(cfg, 'test')


set_mode('eval', [change_detector, speaker])
with torch.no_grad():
    test_iter_start_time = time.time()

    result_sents_pos = {}
    result_sents_neg = {}
    for i, batch in tqdm(enumerate(test_loader)):

        d_feats, nsc_feats, sc_feats, \
        labels, labels_with_ignore, no_chg_labels, no_chg_labels_with_ignore, masks, no_chg_masks, aux_labels_pos, aux_labels_neg, \
        d_img_paths, nsc_img_paths, sc_img_paths = batch

        batch_size = d_feats.size(0)

        d_feats, nsc_feats, sc_feats = d_feats.to(device), nsc_feats.to(device), sc_feats.to(device)
        d_feats, nsc_feats, sc_feats = \
            spatial_info(d_feats), spatial_info(nsc_feats), spatial_info(sc_feats)
        labels, labels_with_ignore, masks = labels.to(device), labels_with_ignore.to(device), masks.to(device)
        no_chg_labels, no_chg_labels_with_ignore, no_chg_masks = no_chg_labels.to(device), no_chg_labels_with_ignore.to(
            device), no_chg_masks.to(device)
        aux_labels_pos, aux_labels_neg = aux_labels_pos.to(device), aux_labels_neg.to(device)

        encoder_output_pos, sim_matrix1_pos, sim_matrix2_pos, bef_pos, aft_pos = change_detector(d_feats, sc_feats)
        encoder_output_neg, sim_matrix1_neg, sim_matrix2_neg, bef_neg, aft_neg = change_detector(d_feats, nsc_feats)

        speaker_output_pos, pos_dynamic_atts = speaker.sample(encoder_output_pos, sample_max=1)

        speaker_output_neg, neg_dynamic_atts = speaker.sample(encoder_output_neg, sample_max=1)

        gen_sents_pos = decode_sequence_transformer(idx_to_word, speaker_output_pos[:, 1:])
        gen_sents_neg = decode_sequence_transformer(idx_to_word, speaker_output_neg[:, 1:])


        for j in range(batch_size):
            gts = decode_sequence_transformer(idx_to_word, labels[j][:, 1:])
            gts_neg = decode_sequence_transformer(idx_to_word, no_chg_labels[j][:, 1:])
            sent_pos = gen_sents_pos[j]
            sent_neg = gen_sents_neg[j]
            image_id = d_img_paths[j].split('_')[-1]

            if args.visualize:
                path_before = d_img_paths[j]
                path_after = sc_img_paths[j]
                img_bef = cv2.imread(path_before)
                img_aft = cv2.imread(path_after)
                
                if img_bef is None or img_aft is None:
                    print(f"Warning: Could not read image {path_before} or {path_after}")
                    continue

                img_bef = cv2.cvtColor(img_bef, cv2.COLOR_BGR2RGB)
                img_aft = cv2.cvtColor(img_aft, cv2.COLOR_BGR2RGB)

                fig, axes = plt.subplots(1, 2, figsize=(12, 6))
                
                axes[0].imshow(img_bef)
                axes[0].set_title("Before", fontsize=14)
                axes[0].axis('off')
                
                axes[1].imshow(img_aft)
                axes[1].set_title("After (Changed)", fontsize=14)
                axes[1].axis('off')

                caption_text = "Change description: " + sent_pos
                
                plt.suptitle(caption_text, fontsize=16, y=0.2)

                vis_filename = f"vis_{image_id}"
                vis_save_path = os.path.join(visualize_save_dir, vis_filename)
                plt.savefig(vis_save_path, bbox_inches='tight', dpi=300)
                plt.close(fig)
                
                # if j == 0:
                #     print(f"Saved visualization to {vis_save_path}")
            result_sents_pos[image_id] = sent_pos
            result_sents_neg[image_id + '_n'] = sent_neg
            image_num = image_id.split('.')[0]
            att_bef_path = os.path.join(att_output_path, image_num + '_before')
            att_aft_path = os.path.join(att_output_path, image_num + '_after')


    test_iter_end_time = time.time() - test_iter_start_time
    print('Test took %.4f seconds' % test_iter_end_time)

    result_save_path_pos = os.path.join(caption_output_path, 'sc_results.json')
    result_save_path_neg = os.path.join(caption_output_path, 'nsc_results.json')
    coco_gen_format_save(result_sents_pos, result_save_path_pos)
    coco_gen_format_save(result_sents_neg, result_save_path_neg)

