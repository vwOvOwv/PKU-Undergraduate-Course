from ast import literal_eval
import copy
import yaml
import numpy as np
from utils.attr_dict import AttrDict
import time

__C = AttrDict()
cfg = __C

# --------------------------------------------------------------------------- #
# general options
# --------------------------------------------------------------------------- #
__C.exp_dir = '/home/data10T/lpy/mml-hw3'
__C.exp_name = ''
__C.gpu_id= [0]

# --------------------------------------------------------------------------- #
# logger options
# --------------------------------------------------------------------------- #
__C.logger = AttrDict()
__C.logger.display_id = 2
__C.logger.display_winsize = 25
__C.logger.display_port = 8097

# --------------------------------------------------------------------------- #
# model options
# --------------------------------------------------------------------------- #
__C.model = AttrDict()
__C.model.type = ''

# Encoder
__C.model.encoder = AttrDict()
__C.model.encoder.type = 'base_encoder'
__C.model.encoder.input_dim = 1024
__C.model.encoder.dim = 512
__C.model.encoder.drop_prob = 0.5

# Change Detector
# __C.model.change_detector = AttrDict()
# __C.model.change_detector.type = 'None'
# __C.model.change_detector.input_dim = 2048
# __C.model.change_detector.dim = 128

# Transformer Encoder
__C.model.transformer_encoder = AttrDict()
__C.model.transformer_encoder.type = 'None'
__C.model.transformer_encoder.input_dim = 2048
__C.model.transformer_encoder.feat_dim = 1024 + 2
__C.model.transformer_encoder.emb_dim = 512
__C.model.transformer_encoder.att_dim = 512
__C.model.transformer_encoder.att_head = 8
__C.model.transformer_encoder.att_layer = 2
__C.model.transformer_encoder.dim = 128

# Transformer Decoder
__C.model.transformer_decoder = AttrDict()
__C.model.transformer_decoder.type = 'None'
__C.model.transformer_decoder.input_dim = 2048
__C.model.transformer_decoder.word_dim = 512
__C.model.transformer_decoder.att_dim = 512
__C.model.transformer_decoder.att_head = 8
__C.model.transformer_decoder.vocab_size = 76
__C.model.transformer_decoder.seq_length = 23
__C.model.transformer_decoder.att_layer = 2
__C.model.transformer_decoder.share_wd_cls_weight = True
__C.model.transformer_decoder.label_smoothing = 0.1
# for sampling
__C.model.transformer_decoder.decoding_constraint = 1
__C.model.transformer_decoder.beam_size = 1
__C.model.transformer_decoder.sample_max = 0 # whether to be greedy or sample from multinomial
__C.model.transformer_decoder.temperature = 1.0
__C.model.transformer_decoder.start_from = 'None'

# Localization
__C.model.localization = AttrDict()
__C.model.localization.input_dim = 2048
__C.model.localization.dim = 512

# Speaker
__C.model.speaker = AttrDict()
__C.model.speaker.type = 'attention'
__C.model.speaker.img_feat_size = 1024
__C.model.speaker.input_dim = 1024
__C.model.speaker.rnn_size = 512
__C.model.speaker.embed_input_dim = 4096
__C.model.speaker.embed_dim = 512
__C.model.speaker.att_hid_size = 512
__C.model.speaker.drop_prob_lm = 0.5
__C.model.speaker.word_embed_size = 300
__C.model.speaker.rnn_num_layers = 1 # currently only supports 1
__C.model.speaker.vocab_size = 60
__C.model.speaker.seq_length = 23 # set it to max_sequence
#__C.model.speaker.subj_seq_length = 6 # set it to max_sequence
#__C.model.speaker.change_seq_length = 6 # set it to max_sequence
# for sampling
__C.model.speaker.decoding_constraint = 1
__C.model.speaker.beam_size = 1
__C.model.speaker.sample_max = 0 # whether to be greedy or sample from multinomial
__C.model.speaker.temperature = 1.0
__C.model.speaker.start_from = 'None'


# --------------------------------------------------------------------------- #
# data options
# --------------------------------------------------------------------------- #
__C.data = AttrDict()

__C.data.dataset = 'rcc_dataset'
__C.data.num_workers = 8
__C.data.default_feature_dir = '/home/data10T/lpy/clevr-change-dataset/features'
__C.data.semantic_feature_dir = '/home/data10T/lpy/clevr-change-dataset/sc_features'
__C.data.nonsemantic_feature_dir = '/home/data10T/lpy/clevr-change-dataset/nsc_features'
__C.data.default_img_dir = '/home/data10T/lpy/clevr-change-dataset/images'
__C.data.semantic_img_dir = '/home/data10T/lpy/clevr-change-dataset/sc_images'
__C.data.nonsemantic_img_dir = '/home/data10T/lpy/clevr-change-dataset/nsc_images'
__C.data.vocab_json = '/home/data10T/lpy/clevr-change-dataset/vocab.json'
__C.data.dep_vocab_json = '/home/data10T/lpy/clevr-change-dataset/dep_vocab.json'
__C.data.change2neg_json = '/home/data10T/lpy/clevr-change-dataset/change2neg_changes.json'
__C.data.splits_json = '/home/data10T/lpy/clevr-change-dataset/splits_subset.json'
__C.data.h5_label_file = '/home/data10T/lpy/clevr-change-dataset/labels.h5'
__C.data.h5_ref_label_file = '/home/data10T/lpy/clevr-change-dataset/ref_labels.h5'
__C.data.type_mapping_json = '/home/data10T/lpy/clevr-change-dataset/type_mapping_v2.json'

__C.data.train = AttrDict()
__C.data.train.batch_size = 128
__C.data.train.seq_per_img = 1
__C.data.train.max_samples = None
# __C.data.train.max_samples = 3000

__C.data.val = AttrDict()
__C.data.val.batch_size = 64
__C.data.val.seq_per_img = 5
__C.data.val.max_samples = None

__C.data.test = AttrDict()
__C.data.test.batch_size = 1
__C.data.test.seq_per_img = 5
__C.data.test.max_samples = None

# --------------------------------------------------------------------------- #
# training options
# --------------------------------------------------------------------------- #
__C.train = AttrDict()

__C.train.snapshot_interval = 1000
__C.train.start_from = None
__C.train.max_iter = 10000
__C.train.log_interval = 50
__C.train.scheduled_sampling_start = 1000000
__C.train.scheduled_sampling_increase_every = 5
__C.train.scheduled_sampling_increase_prob = 0.05
__C.train.scheduled_sampling_max_prob = 0.25

__C.train.kl_div_weight = 1.0

__C.train.optim = AttrDict()
__C.train.optim.type = 'sgdmom'
__C.train.optim.lr = 0.01
__C.train.optim.alpha = 0.9
__C.train.optim.beta = 0.999
__C.train.optim.weight_decay = 5e-4
__C.train.optim.step_size = 15 # decay lr after how many epochs
__C.train.optim.gamma = 0.1 # decay rate
__C.train.optim.epsilon = 1e-08
__C.train.hallucinate_per_iter = 2
__C.train.adapt_per_iter = 2
__C.train.keep_rate = 0.95
__C.train.grad_clip = 1.0


# --------------------------------------------------------------------------- #
# --------------------------------------------------------------------------- #


def merge_cfg_from_file(cfg_filename):
    """Load a yaml config file and merge it into the global config."""
    with open(cfg_filename, 'r') as f:
        yaml_cfg = AttrDict(yaml.load(f,Loader=yaml.FullLoader))
    _merge_a_into_b(yaml_cfg, __C)


def merge_cfg_from_cfg(cfg_other):
    """Merge `cfg_other` into the global config."""
    _merge_a_into_b(cfg_other, __C)


def merge_cfg_from_list(cfg_list):
    """Merge config keys, values in a list (e.g., from command line) into the
    global config. For example, `cfg_list = ['TEST.NMS', 0.5]`.
    """
    assert len(cfg_list) % 2 == 0
    for full_key, v in zip(cfg_list[0::2], cfg_list[1::2]):
        key_list = full_key.split('.')
        d = __C
        for subkey in key_list[:-1]:
            assert subkey in d, 'Non-existent key: {}'.format(full_key)
            d = d[subkey]
        subkey = key_list[-1]
        assert subkey in d, 'Non-existent key: {}'.format(full_key)
        value = _decode_cfg_value(v)
        value = _check_and_coerce_cfg_value_type(
            value, d[subkey], subkey, full_key
        )
        d[subkey] = value


def _merge_a_into_b(a, b, stack=None):
    """Merge config dictionary a into config dictionary b, clobbering the
    options in b whenever they are also specified in a.
    """
    assert isinstance(a, AttrDict), 'Argument `a` must be an AttrDict'
    assert isinstance(b, AttrDict), 'Argument `b` must be an AttrDict'

    for k, v_ in a.items():
        full_key = '.'.join(stack) + '.' + k if stack is not None else k
        # a must specify keys that are in b
        if k not in b:
            raise KeyError('Non-existent config key: {}'.format(full_key))

        v = copy.deepcopy(v_)
        v = _decode_cfg_value(v)
        v = _check_and_coerce_cfg_value_type(v, b[k], k, full_key)

        # Recursively merge dicts
        if isinstance(v, AttrDict):
            try:
                stack_push = [k] if stack is None else stack + [k]
                _merge_a_into_b(v, b[k], stack=stack_push)
            except BaseException:
                raise
        else:
            b[k] = v


def _decode_cfg_value(v):
    """Decodes a raw config value (e.g., from a yaml config files or command
    line argument) into a Python object.
    """
    # Configs parsed from raw yaml will contain dictionary keys that need to be
    # converted to AttrDict objects
    if isinstance(v, dict):
        return AttrDict(v)
    # All remaining processing is only applied to strings
    if not isinstance(v, str):
        return v
    # Try to interpret `v` as a:
    #   string, number, tuple, list, dict, boolean, or None
    try:
        v = literal_eval(v)
    # The following two excepts allow v to pass through when it represents a
    # string.
    #
    # Longer explanation:
    # The type of v is always a string (before calling literal_eval), but
    # sometimes it *represents* a string and other times a data structure, like
    # a list. In the case that v represents a string, what we got back from the
    # yaml parser is 'foo' *without quotes* (so, not '"foo"'). literal_eval is
    # ok with '"foo"', but will raise a ValueError if given 'foo'. In other
    # cases, like paths (v = 'foo/bar' and not v = '"foo/bar"'), literal_eval
    # will raise a SyntaxError.
    except ValueError:
        pass
    except SyntaxError:
        pass
    return v


def _check_and_coerce_cfg_value_type(value_a, value_b, key, full_key):
    """Checks that `value_a`, which is intended to replace `value_b` is of the
    right type. The type is correct if it matches exactly or is one of a few
    cases in which the type can be easily coerced.
    """
    # The types must match (with some exceptions)
    type_b = type(value_b)
    type_a = type(value_a)
    if type_a is type_b:
        return value_a

    # Exceptions: numpy arrays, strings, tuple<->list
    if isinstance(value_b, np.ndarray):
        value_a = np.array(value_a, dtype=value_b.dtype)
    elif isinstance(value_b, str):
        value_a = str(value_a)
    elif isinstance(value_a, tuple) and isinstance(value_b, list):
        value_a = list(value_a)
    elif isinstance(value_a, list) and isinstance(value_b, tuple):
        value_a = tuple(value_a)
    else:
        raise ValueError(
            'Type mismatch ({} vs. {}) with values ({} vs. {}) for config '
            'key: {}'.format(type_b, type_a, value_b, value_a, full_key)
        )
    return value_a
