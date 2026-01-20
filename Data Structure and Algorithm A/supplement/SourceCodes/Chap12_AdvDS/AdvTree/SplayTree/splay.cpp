//伸展树的使用代码
//本例是用来统计一个文件中的单词
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include <cstdlib> 
#include "Splay.h"

using namespace std;

class Word 
{//用来测试存储单词的类
public:
    Word() 
	{
        freq = 1;
    }
    int operator== (const Word& ir) const {
        return strcmp(word,ir.word) == 0;
    }
    int operator< (const Word& ir) const {
        return strcmp(word,ir.word) < 0;
    }
private:
    char *word;  //单词
    int freq;    //使用频率
    friend class WordSplay;
};

class WordSplay : public SplayTree<Word> 
{//使用伸展树构造一颗用于处理单词的树
public:
    WordSplay() 
	{
        differentWords = wordCnt = 0;
    }
    void run(ifstream&,char*);
private:
    int differentWords, // 不同的单词数
        wordCnt;        // 同一个单词出现最多的数量
    void visit(SplayingNode<Word>*);
};

void WordSplay::visit(SplayingNode<Word> *p) 
{
    differentWords++;
    wordCnt += p->info.freq;
}

void WordSplay::run(ifstream& fIn, char *fileName) 
{
    char ch = ' ', i;
    char s[100];
    Word rec;
    while (!fIn.eof()) {
        while (1)
            if (!fIn.eof() && !isalpha(ch)) // 掠过非字符
                 fIn.get(ch);
            else break;
        if (fIn.eof())       
             break;
        for (i = 0; !fIn.eof() && isalpha(ch); i++) {
             s[i] = toupper(ch);
             fIn.get(ch);
        }
        s[i] = '\0';
        if (!(rec.word = new char[strlen(s)+1])) {
             cerr << "No room for new words.\n";
             exit(1);
        }
        strcpy(rec.word,s);
        Word *p = search(rec);
        if (p == 0)
             insert(rec);
        else p->freq++;
    }
    inorder();
    cout << "\n\nFile " << fileName
         << " contains " << wordCnt << " words among which "
         << differentWords << " are different\n";
}

int main(int argc, char* argv[])
{
    char fileName[80];
    WordSplay splayTree;
    if (argc != 2) {
         cout << "输入一个用来统计的文件: ";
         cin  >> fileName;
    }
    else strcpy(fileName,argv[1]);
    ifstream fIn(fileName);
    if (fIn.fail()) {
        cerr << "Cannot open " << fileName << endl;
        return 1;
    }
    splayTree.run(fIn,fileName);
    fIn.close();
    return 0;
}
