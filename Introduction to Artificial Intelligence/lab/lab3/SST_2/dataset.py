import os
import random
import string
import nltk
import pickle

class basedataset():
    def __init__(self, mode, maxlen=None):
        assert mode in ['train', 'test', 'dev']
        self.root = './SST_2/'+mode+'.tsv'
        f = open(self.root, 'r', encoding='utf-8')
        L = f.readlines()
        self.data = [x.strip().split('\t') for x in L]
        if maxlen is not None:
            self.data = self.data[:maxlen]
        self.len = len(self.data)
        self.count = 0

    def tokenize(self, text):
        cleaned_tokens = []
        tokens = nltk.tokenize.word_tokenize(text.lower())#将文本分解为单词和标点符号，并将它们作为独立的词元返回。
        for token in tokens:
            if token in nltk.corpus.stopwords.words('english'):#检查当前词元是否在 NLTK 提供的英语停用词列表中
                continue
            else:
                all_punct = True
                for char in token:#检查该词元是不是“用于断句”的标点符号
                    if char not in string.punctuation:#不是用于断句的标点符号（"10.99"）
                        all_punct = False
                        break
                if not all_punct:
                    cleaned_tokens.append(token)
        return cleaned_tokens

    def __getitem__(self, index):
        # print("called")
        text, label = self.data[index]
        text = text.strip()
        text = self.tokenize(text)
        return text, int(label)
    
    def get(self, index):
        text, label = self.data[index]
        return text, int(label)

def traindataset():
    return basedataset('train')

def minitraindataset():
    return basedataset('train', maxlen=100)

def testdataset():
    return basedataset('dev')

def validationdataset():
    return basedataset('dev')

if __name__ == '__main__':
    pass