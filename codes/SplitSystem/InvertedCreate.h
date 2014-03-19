#pragma once

#include "SplitText.h"
#include "InvertedEntry.h"
#include <string>
#include <list>
using namespace std;

/************************************************************************/
/* ����������������������ļ���ͬʱ�ṩ�����Ժ�ĵ����ļ�����Ϣ         */
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
	bool isCreate;					/*����ָʾ�����ļ��Ƿ��Ѿ�����*/
	unsigned int totalDocNum;		/*���������ļ�ʱ�ı������ļ���*/
	SplitText splitText;			/*�������ı��ִʵ���*/
	map<string, InvertedEntry> invertedMap;		/*�����ŵ���������*/
};
