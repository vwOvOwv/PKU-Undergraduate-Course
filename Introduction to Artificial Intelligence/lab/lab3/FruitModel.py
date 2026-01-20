import math
from SST_2.dataset import traindataset, minitraindataset
from fruit import get_document, tokenize
import pickle
import numpy as np
from importlib.machinery import SourcelessFileLoader
from autograd.BaseGraph import Graph
from autograd.BaseNode import *

class NullModel:
    def __init__(self):
        pass

    def __call__(self, text):
        return 0


class NaiveBayesModel:
    def __init__(self):
        self.dataset = traindataset() # 完整训练集
        # self.dataset = minitraindataset() # 用来调试的小训练集
        # 以下内容可根据需要自行修改
        self.token_num = [{}, {}] # token在正负样本中出现次数
        self.V = 0 #语料库token数量
        self.pos_neg_num = [0, 0] # 正负样本数量
        self.sum=0
        self.alpha=0.2#平滑超参数
        self.count()

    def count(self):
        # TODO: YOUR CODE HERE
        # 提示：统计token分布不需要返回值
        for text,label in self.dataset:
            self.pos_neg_num[label]+=1
            for token in text:
                if token in self.token_num[label]:
                    self.token_num[label][token]+=1
                else:
                    self.token_num[label][token]=1
        self.sum=self.pos_neg_num[0]+self.pos_neg_num[1]
        self.V=len(self.token_num[0])+len(self.token_num[1])
        # print(self.V)

    def __call__(self, text):
        # TODO: YOUR CODE HERE
        pLike=self.pos_neg_num[1]/self.sum
        pDislike=self.pos_neg_num[0]/self.sum
        for token in text:
            pTokenLike=self.alpha/(self.pos_neg_num[1]+self.alpha*self.V)
            pTokenDislike=self.alpha/(self.pos_neg_num[0]+self.alpha*self.V)
            if token in self.token_num[1]:
                pTokenLike=(self.token_num[1][token]+self.alpha)/(self.pos_neg_num[1]+self.alpha*self.V)
            if token in self.token_num[0]:
                pTokenDislike=(self.token_num[0][token]+self.alpha)/(self.pos_neg_num[0]+self.alpha*self.V)
            pLike*=pTokenLike
            pDislike*=pTokenDislike
        # print(pLike,pDislike)
        if pLike>pDislike:
            return 1
        else:
            return 0
        # 返回1或0代表当前句子分类为正/负样本
        raise NotImplementedError


def buildGraph(dim, num_classes): #dim: 输入一维向量长度， num_classes:分类数
    # TODO: YOUR CODE HERE
    # 填写网络结构，请参考lab2相关部分
    # 填写代码前，请确定已经将lab2写好的BaseNode.py与BaseGraph.py放在autograd文件夹下
    nodes = []
    nodes.append(Linear(dim,70))
    nodes.append(Dropout())
    nodes.append(Linear(70,40))
    nodes.append(Dropout())
    nodes.append(Linear(40,20))
    nodes.append(Dropout())
    nodes.append(Linear(20,num_classes))
    nodes.append(Softmax())
    nodes.append(NLLLoss(0))
    graph = Graph(nodes)
    return graph

save_path = "model/mlp.npy"

class Embedding():
    def __init__(self):
        self.emb = dict() 
        with open("words.txt", encoding='utf-8') as f: #word.txt存储了每个token对应的feature向量，self.emb是一个存储了token-feature键值对的Dict()，可直接调用使用
            for i in range(50000):
                row = next(f).split()
                word = row[0]
                vector = np.array([float(x) for x in row[1:]])
                self.emb[word] = vector
        
    def __call__(self, text):
        # TODO: YOUR CODE HERE
        # 利用self.emb将句子映射为一个一维向量，注意，同时需要修改训练代码中的网络维度部分
        # 数据格式可以参考lab2
        vec=np.array([0.0 for i in range(100)])
        for token in text:
            if token in self.emb:
                # print(self.emb[token])
                vec+=self.emb[token]
        vec=np.reshape(vec,(1,100))
        # print(vec)
        return vec
        raise NotImplementedError


class MLPModel():
    def __init__(self):
        self.embedding = Embedding()
        with open(save_path, "rb") as f:
            self.network = pickle.load(f)
        self.network.eval()
        self.network.flush()

    def __call__(self, text):
        X = self.embedding(text)
        pred = self.network.forward(X, removelossnode=1)[-1]
        haty = np.argmax(pred, axis=1)
        return haty[0]


class QAModel():
    def __init__(self):
        self.document_list = get_document()

    def tf(self, word, document):
        # TODO: YOUR CODE HERE
        # 返回单词在文档中的频度
        N=len(document)
        n=0
        for token in document:
            if word==token:
                n+=1
        return math.log10(n/N+1)
        raise NotImplementedError  

    def idf(self, word):
        # TODO: YOUR CODE HERE
        # 返回单词IDF值，提示：你需要利用self.document_list来遍历所有文档
        documents=self.document_list
        D=len(documents)
        d=0
        for document in documents:
            tokens=document.keys()
            if word in tokens:
                d+=1
        return math.log10(D/(1+d))
        raise NotImplementedError  
    
    def tfidf(self, word, document):
        # TODO: YOUR CODE HERE
        # 返回TF-IDF值
        tf=self.tf(word,document)
        idf=self.idf(word)
        return tf*idf
        raise NotImplementedError  

    def __call__(self, query):
        query = tokenize(query) # 将问题token化
        # print(query)
        # TODO: YOUR CODE HERE
        # 利用上述函数来实现QA
        # 提示：你需要根据TF-IDF值来选择一个最合适的文档，再根据TF-IDF值选择最合适的句子
        # 返回时请返回原本句子，而不是token化后的句子，可以参考README中数据结构部分以及fruit.py中用于数据处理的get_document()函数
        documents=get_document()
        maxTfidf=float('-inf')
        for document in documents:
            documentTokens=document['document']
            tfidf=0
            for queryToken in query:
                tfidf+=self.tfidf(queryToken,documentTokens)
            if tfidf>maxTfidf:
                answerDocument=document
                maxTfidf=tfidf

        sentences=answerDocument['sentences']
        Idf={}
        D=len(sentences)
        for queryToken in query:
            d=0
            for sentence in sentences:
                if queryToken in sentence[0]:
                    d+=1
            Idf[queryToken]=math.log10(D/(1+d))
        maxTfidf=float('-inf')
        for sentence in sentences:
            sentenceTokens=sentence[0]
            tfidf=0
            for queryToken in query:
                tf=self.tf(queryToken,sentenceTokens)
                tfidf+=tf*Idf[queryToken]
            if tfidf>maxTfidf:
                maxTfidf=tfidf
                answerSentence=sentence    
        return answerSentence[1]
        raise NotImplementedError
            

modeldict = {
    "Null": NullModel,
    "Naive": NaiveBayesModel,
    "MLP": MLPModel,
    "QA": QAModel,
}


if __name__ == '__main__':
    embedding = Embedding()
    lr = 1e-3   # 学习率
    wd1 = 1e-4  # L1正则化
    wd2 = 1e-4  # L2正则化
    batchsize = 64
    max_epoch = 10
    dp = 0.1
    graph = buildGraph(100, 2) # 维度需要自己修改

    # 训练
    best_train_acc = 0
    dataloader = traindataset() # 完整训练集
    # dataloader = minitraindataset() # 用来调试的小训练集
    for i in range(1, max_epoch+1):
        hatys = []
        ys = []
        losss = []
        graph.train()
        X = []
        Y = []
        cnt = 0
        for text, label in dataloader:
            x = embedding(text)
            label = np.zeros((1)).astype(np.int32) + label
            X.append(x)
            Y.append(label)
            cnt += 1
            if cnt == batchsize:
                X = np.concatenate(X, 0)
                Y = np.concatenate(Y, 0)
                graph[-1].y = Y
                graph.flush()
                pred, loss = graph.forward(X)[-2:]
                hatys.append(np.argmax(pred, axis=1))
                ys.append(Y)
                graph.backward()
                graph.optimstep(lr, wd1, wd2)
                losss.append(loss)
                cnt = 0
                X = []
                Y = []

        loss = np.average(losss)
        acc = np.average(np.concatenate(hatys)==np.concatenate(ys))
        print(f"epoch {i} loss {loss:.3e} acc {acc:.4f}")
        if acc > best_train_acc:
            best_train_acc = acc
            with open(save_path, "wb") as f:
                pickle.dump(graph, f)