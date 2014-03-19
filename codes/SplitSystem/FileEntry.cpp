#include "FileEntry.h"

FileEntry::FileEntry(const string& fileName)
{
	this->fileName = fileName;
	this->freq = 1;		/*数据结构的创建就代表有某个词在遍历文件时新出现了*/
}

FileEntry& FileEntry::operator=(const FileEntry& copy)
{
	this->fileName = copy.fileName;
	this->freq = copy.freq;
	return *this;
}

/************************************************************************/
/* FileEntry的拷贝构造函数                                                                     */
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
/* 在FileEntry所对应的文件中，与这个FileEntry对应的term频率加1*/
/************************************************************************/
void FileEntry::addFreq()
{
	this->freq++;
}
