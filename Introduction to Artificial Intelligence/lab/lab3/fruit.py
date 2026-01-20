from SST_2.dataset import testdataset
import os
import nltk
import string

fruit_dataset = testdataset()


def getdata(index):
    text, label = fruit_dataset.__getitem__(index)
    return text, label


def tokenize(text):
    cleaned_tokens = []
    tokens = nltk.tokenize.word_tokenize(text.lower())
    for token in tokens:
        if token in nltk.corpus.stopwords.words('english'):
            continue
        else:
            all_punct = True
            for char in token:
                if char not in string.punctuation:
                    all_punct = False
                    break
            if not all_punct:
                cleaned_tokens.append(token)
    return cleaned_tokens


def get_document(root='./qadata'):
    cleaned_tokens = []
    document_list = os.listdir(root)
    document_list.sort()
    all_documents = []
    for path in document_list:
        path = os.path.join(root, path)
        
        with open(path, 'r', encoding='utf-8') as file:
            document = file.read()

        # tokenize document
        cleaned_tokens = tokenize(document)
        now_document = {}
        now_document['document'] = cleaned_tokens

        # tokenize sentences
        sentences = []
        for passage in document.split("\n"):
            for sentence in nltk.sent_tokenize(passage):
                tokens = tokenize(sentence)
                sentences.append([tokens, sentence])

        now_document['sentences'] = sentences
        all_documents.append(now_document)
    return all_documents

