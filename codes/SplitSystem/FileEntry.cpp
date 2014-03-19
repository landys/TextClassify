#include "FileEntry.h"

FileEntry::FileEntry(const string& fileName)
{
	this->fileName = fileName;
	this->freq = 1;		/*���ݽṹ�Ĵ����ʹ�����ĳ�����ڱ����ļ�ʱ�³�����*/
}

FileEntry& FileEntry::operator=(const FileEntry& copy)
{
	this->fileName = copy.fileName;
	this->freq = copy.freq;
	return *this;
}

/************************************************************************/
/* FileEntry�Ŀ������캯��                                                                     */
/************************************************************************/
FileEntry::FileEntry(const FileEntry& fileEntry)
{
	this->fileName = fileEntry.fileName;
	this->freq = fileEntry.freq;
}

FileEntry::~FileEntry(void)
{
}

/************************************************************************/
/* ��FileEntry����Ӧ���ļ��У������FileEntry��Ӧ��termƵ�ʼ�1*/
/************************************************************************/
void FileEntry::addFreq()
{
	this->freq++;
}
