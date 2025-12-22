"""
Utilities for preprocessing sequence data.

Special tokens that are in all dictionaries:

<NULL>: Extra parts of the sequence that we should ignore
<START>: Goes at the start of a sequence
<END>: Goes at the end of a sequence, before <NULL> tokens
<UNK>: Out-of-vocabulary words
"""

import spacy
import textacy
from spacy.matcher import Matcher
from spacy.util import filter_spans

phrase = []
nlp = spacy.load('en_core_web_sm')
pattern = [{'POS': 'VERB', 'OP': '?'},
           {'POS': 'ADV', 'OP': '*'},
           {'POS': 'AUX', 'OP': '*'},
           {'POS': 'VERB', 'OP': '+'}]

matcher = Matcher(nlp.vocab)
matcher.add("Verb phrase", [pattern])

SPECIAL_TOKENS = {
  '<NULL>': 0,
  '<UNK>': 1,
  '<START>': 2,
  '<END>': 3,
}


def tokenize(s, delim=' ',
      add_start_token=True, add_end_token=True,
      punct_to_keep=None, punct_to_remove=None):
  """
  Tokenize a sequence, converting a string s into a list of (string) tokens by
  splitting on the specified delimiter. Optionally keep or remove certain
  punctuation marks and add start and end tokens.
  """
  if punct_to_keep is not None:
    for p in punct_to_keep:
      s = s.replace(p, '%s%s' % (delim, p))

  if punct_to_remove is not None:
    for p in punct_to_remove:
      s = s.replace(p, '')

  tokens = s.split(delim)
  if add_start_token:
    tokens.insert(0, '<START>')
  if add_end_token:
    tokens.append('<END>')
  return tokens


def tokenize_phrase(s, delim=' ',
      add_start_token=True, add_end_token=True,
      punct_to_keep=None, punct_to_remove=None):
  """
  Tokenize a sequence, converting a string s into a list of (string) tokens by
  splitting on the specified delimiter. Optionally keep or remove certain
  punctuation marks and add start and end tokens.
  """
  if punct_to_keep is not None:
    for p in punct_to_keep:
      s = s.replace(p, '%s%s' % (delim, p))

  if punct_to_remove is not None:
    for p in punct_to_remove:
      s = s.replace(p, '')
  # phrase = []
  doc = nlp(s)
  # call the matcher to find matches
  # matches = matcher(doc)
  # spans = [doc[start:end] for _, start, end in matches]
  noun_chunks = [nc for nc in doc.noun_chunks]
  phrase = noun_chunks
  # phrase.extend(noun_chunks)
  # phrase.extend(filter_spans(spans))
  if add_start_token:
    phrase.insert(0, '<START>')
  if add_end_token:
    phrase.append('<END>')
  return phrase

def dep_tokenize(s, delim=' ',
      add_start_token=True, add_end_token=True,
      punct_to_keep=None, punct_to_remove=None):
  """
  Tokenize a sequence, converting a string s into a list of (string) tokens by
  splitting on the specified delimiter. Optionally keep or remove certain
  punctuation marks and add start and end tokens.
  """

  if add_start_token:
    s.insert(0, '<START>')
  if add_end_token:
    s.append('<END>')
  return s

def build_dep_vocab():  # 47 in total, all tags plus pad and non-connect
    '''
    biaffine dep_tag Vocab : {0: 'punct', 1: 'prep', 2: 'pobj', 3: 'det', 4: 'nn',
        5: 'nsubj', 6: 'amod', 7: 'root', 8: 'dobj', 9: 'aux', 10: 'advmod', 11: 'conj',
        12: 'cc', 13: 'num', 14: 'poss', 15: 'ccomp', 16: 'dep', 17: 'xcomp', 18: 'mark',
        19: 'cop', 20: 'number', 21: 'possessive', 22: 'rcmod', 23: 'auxpass', 24: 'appos',
        25: 'nsubjpass', 26: 'advcl', 27: 'partmod', 28: 'pcomp', 29: 'neg', 30: 'tmod',
        31: 'quantmod', 32: 'npadvmod', 33: 'prt', 34: 'infmod', 35: 'parataxis',
        36: 'mwe', 37: 'expl', 38: 'acomp', 39: 'iobj', 40: 'csubj', 41: 'predet',
        42: 'preconj', 43: 'discourse', 44: 'csubjpass'}
    This is used in energy case.
    '''

    head_tags = {0: 'punct', 1: 'prep', 2: 'pobj', 3: 'det', 4: 'nn', 5: 'nsubj', 6: 'amod', 7: 'root', 8: 'dobj', 9: 'aux', 10: 'advmod', 11: 'conj', 12: 'cc', 13: 'num', 14: 'poss', 15: 'ccomp', 16: 'dep', 17: 'xcomp', 18: 'mark', 19: 'cop', 20: 'number', 21: 'possessive', 22: 'rcmod', 23: 'auxpass', 24: 'appos',
                 25: 'nsubjpass', 26: 'advcl', 27: 'partmod', 28: 'pcomp', 29: 'neg', 30: 'tmod', 31: 'quantmod', 32: 'npadvmod', 33: 'prt', 34: 'infmod', 35: 'parataxis', 36: 'mwe', 37: 'expl', 38: 'acomp', 39: 'iobj', 40: 'csubj', 41: 'predet', 42: 'preconj', 43: 'discourse', 44: 'csubjpass'}

    itos = [head_tags[i] for i in range(len(head_tags))]
    stoi = {}
    for token, idx in SPECIAL_TOKENS.items():
      stoi[token] = idx
    stoi.update({tok: i+4 for i, tok in enumerate(itos)})
    return stoi


def build_vocab(sequences, min_token_count=1, delim=' ',
                punct_to_keep=None, punct_to_remove=None):
  token_to_count = {}
  tokenize_kwargs = {
    'delim': delim,
    'punct_to_keep': punct_to_keep,
    'punct_to_remove': punct_to_remove,
  }
  for seq in sequences:
    seq_tokens = tokenize(seq, **tokenize_kwargs,
                    add_start_token=False, add_end_token=False)
    for token in seq_tokens:
      if token not in token_to_count:
        token_to_count[token] = 0
      token_to_count[token] += 1

  token_to_idx = {}
  for token, idx in SPECIAL_TOKENS.items():
    token_to_idx[token] = idx
  for token, count in sorted(token_to_count.items()):
    if count >= min_token_count:
      token_to_idx[token] = len(token_to_idx)

  return token_to_idx

def build_phrase_vocab(sequences, min_token_count=1, delim=' ',
                punct_to_keep=None, punct_to_remove=None):
  token_to_count = {}
  tokenize_kwargs = {
    'delim': delim,
    'punct_to_keep': punct_to_keep,
    'punct_to_remove': punct_to_remove,
  }
  for seq in sequences:
    seq_tokens = tokenize_phrase(seq, **tokenize_kwargs,
                    add_start_token=False, add_end_token=False)
    for token in seq_tokens:
      if token not in token_to_count:
        token_to_count[token] = 0
      token_to_count[token] += 1

  token_to_idx = {}
  for token, idx in SPECIAL_TOKENS.items():
    token_to_idx[token] = idx
  for token, count in sorted(token_to_count.items()):
    if count >= min_token_count:
      token_to_idx[token] = len(token_to_idx)

  return token_to_idx


def encode(seq_tokens, token_to_idx, allow_unk=False):
  seq_idx = []
  for token in seq_tokens:
    if token not in token_to_idx:
      if allow_unk:
        token = '<UNK>'
      else:
        raise KeyError('Token "%s" not in vocab' % token)
    seq_idx.append(token_to_idx[token])
  return seq_idx


def decode(seq_idx, idx_to_token, delim=None, stop_at_end=True):
  tokens = []
  for idx in seq_idx:
    tokens.append(idx_to_token[idx])
    if stop_at_end and tokens[-1] == '<END>':
      break
  if delim is None:
    return tokens
  else:
    return delim.join(tokens)
