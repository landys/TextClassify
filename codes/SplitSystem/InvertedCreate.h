#pragma once

#include "SplitText.h"
#include "InvertedEntry.h"
#include <string>
#include <list>
using namespace std;

/************************************************************************/
/* 这个类用来创建倒排索引文件，同时提供创建以后的倒排文件的信息         */
/************************************************************************/
class InvertedCreate
{
public:
	InvertedCreate(const string& componentPath);
	~InvertedCreate(void);
	const map<string, InvertedEntry>& createInvertedFile(const map<int, string>& trainSet);
	unsigned int getTotalDocNum() const;
	unsigned int getTermDocNum(const string& term) const;


private:
	void addInvertedEntry(const string& word, const string& fileName, const int type);
	void insertWord(const string& word, const string& fileName, const int type);
	void insertWordsToInvertedFile(const list<string>& wordList, const string& fileName, const int type);
	void createInvertedEntry(const list<string>& fileList, int type);
	string getMapKey(const string& word, const int type) const;
	void showResult() const;

private:
	bool isCreate;					/*用来指示倒排文件是否已经创建*/
	unsigned int totalDocNum;		/*创建倒排文件时的遍历的文件数*/
	SplitText splitText;			/*用来对文本分词的类*/
	map<string, InvertedEntry> invertedMap;		/*保存着倒排索引项*/
};
