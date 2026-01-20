import sys
COCO_PATH = '/data/coco-caption_python3/' # i.e. /home/user/code/coco-caption
sys.path.insert(0, COCO_PATH)

import json
import copy
import numpy as np

from pycocotools.coco import COCO
from pycocoevalcap.eval import COCOEvalCap

def coco_gt_format_save(gt_file, neg=False):
    gt = json.load(open(gt_file, 'r'))
    gt_dict = {}
    info_dict = {
        'contributor': 'dummy',
        'date_created': 'dummy',
        'description': 'dummy',
        'url': 'dummy',
        'version': 'dummy',
        'year': 'dummy'
    }

    gt_dict['info'] = info_dict
    gt_dict['licenses'] = info_dict
    gt_dict['type'] = 'captions'
    gt_dict['images'] = []
    gt_dict['annotations'] = []

    count = 0
    for k, v in gt.items():
        image_id = k.split('_')[-1]
        if neg:
            image_id = image_id + '_n'

        im = {'filename': image_id, 'id': image_id}
        gt_dict['images'].append(im)
        for c in v:
            annotation = {'caption': c, 'id': count, 'image_id': image_id}
            count += 1
            gt_dict['annotations'].append(annotation)

    json.dump(gt_dict, open(gt_file.split('.json')[0] + '_reformat.json', 'w'))

def coco_gen_format(gen_dict):
    results = []
    for k, v in gen_dict.items():
        results.append({'caption': v, 'image_id': k})
    return results

def coco_gen_format_save(gen_dict, save_path):
    results = coco_gen_format(gen_dict)
    json.dump(results, open(save_path, 'w'))

def merge_gt_files(gt_file1, gt_file2, save_path):
    gt1 = json.load(open(gt_file1, 'r'))
    gt2 = json.load(open(gt_file2, 'r'))
    gt_dict = {}
    info_dict = {
        'contributor': 'dummy',
        'date_created': 'dummy',
        'description': 'dummy',
        'url': 'dummy',
        'version': 'dummy',
        'year': 'dummy'
    }

    gt_dict['info'] = info_dict
    gt_dict['licenses'] = info_dict
    gt_dict['type'] = 'captions'
    gt_dict['images'] = []
    gt_dict['annotations'] = []

    count = 0
    for k, v in gt1.items():
        image_id = k.split('_')[-1]
        im = {'filename': image_id, 'id': image_id}
        gt_dict['images'].append(im)
        for c in v:
            annotation = {'caption': c, 'id': count, 'image_id': image_id}
            count += 1
            gt_dict['annotations'].append(annotation)

    for k, v in gt2.items():
        image_id = k.split('_')[-1] + '_n'
        im = {'filename': image_id, 'id': image_id}
        gt_dict['images'].append(im)
        for c in v:
            annotation = {'caption': c, 'id': count, 'image_id': image_id}
            count += 1
            gt_dict['annotations'].append(annotation)

    json.dump(gt_dict, open(save_path, 'w'))

def score_generation(anno_file, result_file):
    coco = COCO(anno_file)
    coco_res = coco.loadRes(result_file)

    coco_eval = COCOEvalCap(coco, coco_res)
    coco_eval.params['image_id'] = coco_res.getImgIds()

    coco_eval.evaluate()
    return copy.deepcopy(coco_eval.eval)

def score_generation_by_type(anno_file, result_file, type_file):
    coco = COCO(anno_file)
    coco_res = coco.loadRes(result_file)
    coco_eval = COCOEvalCap(coco, coco_res)

    type_dict = json.load(open(type_file, 'r'))
    results = {}
    for type, image_ids in type_dict.items():
        filtered = set(coco_res.getImgIds()).intersection(set(image_ids))
        coco_eval.params['image_id'] = list(filtered)
        coco_eval.evaluate()
        results[type] = copy.deepcopy(coco_eval.eval)

    return results

def score_generation_with_ids(anno_file, result_file, img_ids):
    coco = COCO(anno_file)
    coco_res = coco.loadRes(result_file)

    coco_eval = COCOEvalCap(coco, coco_res)
    filtered = set(coco_res.getImgIds()).intersection(set(img_ids))
    coco_eval.params['image_id'] = list(filtered)

    coco_eval.evaluate()
    return copy.deepcopy(coco_eval.eval)

def score_generation_by_type_with_ids(anno_file, result_file, type_file, img_ids):
    coco = COCO(anno_file)
    coco_res = coco.loadRes(result_file)
    coco_eval = COCOEvalCap(coco, coco_res)

    type_dict = json.load(open(type_file, 'r'))
    results = {}
    for type, image_ids in type_dict.items():
        filtered = set(coco_res.getImgIds()).intersection(set(image_ids))
        filtered_twice = filtered.intersection(set(img_ids))
        coco_eval.params['image_id'] = list(filtered_twice)
        coco_eval.evaluate()
        results[type] = copy.deepcopy(coco_eval.eval)

    return results

def pointing(gen_mapping, gt_mapping, type_ids=None):
    pointings = []
    count = 0
    if type_ids:
        type_ids = set([str(int(id.split('.')[0])) for id in type_ids])
    for id, (gen_before, gen_after) in gen_mapping.items():
        if type_ids and id not in type_ids:
            continue
        gt_before, gt_after = gt_mapping[id]
        if gt_before is not None:
            gen_before_flat = gen_before.flatten()
            gt_before_flat = gt_before.flatten()
            p_before = gt_before_flat[np.argmax(gen_before_flat)]
            count += 1
        else:
            p_before = 0.0
        if gt_after is not None:
            gen_after_flat = gen_after.flatten()
            gt_after_flat = gt_after.flatten()
            p_after = gt_after_flat[np.argmax(gen_after_flat)]
            count += 1
        else:
            p_after = 0.0
        p = p_before + p_after
        pointings.append(p)
    m_pointing = sum(pointings) / float(count)
    return m_pointing

def coverage(gen_mapping, gt_mapping, type_ids=None):
    coverages = []
    if type_ids:
        type_ids = set([str(int(id.split('.')[0])) for id in type_ids])
    for id, (gen_before, gen_after) in gen_mapping.items():
        # normalize
        gen_before = gen_before / gen_before.sum()
        gen_after = gen_after / gen_after.sum()
        if type_ids and id not in type_ids:
            continue
        gt_before, gt_after = gt_mapping[id]
        if gt_before is not None:
            s_before = (gt_before * gen_before).sum()
        else:
            s_before = 0.0
        if gt_after is not None:
            s_after = (gt_after * gen_after).sum()
        else:
            s_after = 0.0
        score = (s_before + s_after) / 2.0
        coverages.append(score)
    m_coverage = np.mean(coverages)
    return m_coverage


if __name__ == '__main__':
    anno_path = './data/change_captions.json'
    coco_gt_format_save(anno_path)
    anno_neg_path = './data/no_change_captions.json'
    coco_gt_format_save(anno_neg_path, neg=True)

    save_path = './data/total_change_captions_reformat.json'
    merge_gt_files(anno_path, anno_neg_path, save_path)
