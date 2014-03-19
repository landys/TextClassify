#pragma once

#include "FileEntry.h"
#include <string>
#include <map>
using namespace std;

/************************************************************************/
/* ������ʾ�����ļ������ļ��е�һ����ڵ����ļ��ĸ�ʽΪ��
* ���ļ���term(����), type(���)�� freq(�����ֵ���Ƶ��)�� docNum(���ֵ��ļ���)
* ���ļ���term(����), fileName(���ֵ��ļ���)��freq(���ļ��г��ֵ�Ƶ��)
* ��������ĳ��������ĳ�������ļ������г�����Ƶ��
*/
/************************************************************************/
class InvertedEntry
{
public:
	InvertedEntry(const string& term, int classifiedType);
	InvertedEntry(const InvertedEntry& invertedEntry);
	InvertedEntry(const string& term, const string& fileName, const int classifiedType);
	InvertedEntry& operator=(const InvertedEntry& copy);
	void addFreq(const string& fileName);
	unsigned int getDocNum() const;
	~InvertedEntry(void);
public:
	string term;		/*�����Ĵ���*/
	int classifiedType;	/*������������Ǵ������ļ�����ȡ������*/
	unsigned int totalFreq;		/*��������������ļ��г��ֵ���Ƶ��*/
	map<string, FileEntry> fileEntryMap;	/*�������Ӧ�ĸ��ļ��е���*/

private:
	void addFileEntry(const string& fileName);
};
