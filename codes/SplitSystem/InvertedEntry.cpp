#include "InvertedEntry.h"

/************************************************************************/
/* Ĭ�Ͽ������캯��                                                                     */
/************************************************************************/
InvertedEntry::InvertedEntry(const InvertedEntry& invertedEntry)
{
	this->term = invertedEntry.term;
	this->classifiedType = invertedEntry.classifiedType;
	this->totalFreq = invertedEntry.totalFreq;

	map<string, FileEntry>::const_iterator pos;
	for(pos = invertedEntry.fileEntryMap.begin(); pos != invertedEntry.fileEntryMap.end(); pos++)
	{// Ҫ�ֶ��������ļ��е�����Կ�����copy����
		this->fileEntryMap.insert(make_pair(pos->first, pos->second));
	}
}

/************************************************************************/
/* ����ʹ�õĹ��캯��                                                                     */
/************************************************************************/
InvertedEntry::InvertedEntry(const string& term, const string& fileName, const int classifiedType)
{
	this->term = term;
	this->classifiedType = classifiedType;
	this->totalFreq = 1;/*����ʱ�ͱ�ʾ�г�������ʵ�һ����*/
	
	/*�������ļ�����*/
	FileEntry fileEntry(fileName);
	fileEntryMap.insert(make_pair(fileName, fileEntry));
}

InvertedEntry::~InvertedEntry(void)
{
}

InvertedEntry& InvertedEntry::operator=(const InvertedEntry& copy)
{
	this->term = copy.term;
	this->classifiedType = copy.classifiedType;
	this->totalFreq = copy.totalFreq;

	map<string, FileEntry>::const_iterator pos;
	this->fileEntryMap.clear();
	for(pos = copy.fileEntryMap.begin(); pos != copy.fileEntryMap.end(); pos++)
	{// �ֶ�������T_T,�Ժ�Ҫ��copy
		this->fileEntryMap.insert(make_pair(pos->first, pos->second));
	}
	return *this;
}

/************************************************************************/
/* �õ����������һ���г��ֵ��ļ���                                                                     */
/************************************************************************/
unsigned int InvertedEntry::getDocNum() const
{
	/*���Լ򵥵ĺ͸��ļ�����������Ӧ����������ʾ
	* ��Ϊ���ļ���ÿһ��ͱ�ʾһ���ļ�
	*/
	return fileEntryMap.size();
}

/************************************************************************/
/* �������������һ���г��ֵ�Ƶ��                                                                     */
/************************************************************************/
void InvertedEntry::addFreq(const string& fileName)
{
	totalFreq++;

	map<string, FileEntry>::iterator pos = fileEntryMap.find(fileName);
	if(pos == fileEntryMap.end())
	{// ����ڸ��ļ���û����Ӧ��һ��������һ��
		FileEntry fileEntry(fileName);
		fileEntryMap.insert(make_pair(fileName, fileEntry));
	}
	else
	{// ������ļ����Ѿ���������һ��ͼ򵥶�����Ƶ�ʼ�һ
		pos->second.addFreq();
	}
}
