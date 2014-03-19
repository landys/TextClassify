#include "InvertedEntry.h"

/************************************************************************/
/* 默认拷贝构造函数                                                                     */
/************************************************************************/
InvertedEntry::InvertedEntry(const InvertedEntry& invertedEntry)
{
	this->term = invertedEntry.term;
	this->classifiedType = invertedEntry.classifiedType;
	this->totalFreq = invertedEntry.totalFreq;

	map<string, FileEntry>::const_iterator pos;
	for(pos = invertedEntry.fileEntryMap.begin(); pos != invertedEntry.fileEntryMap.end(); pos++)
	{// 要手动拷贝副文件中的项，可以考虑用copy函数
		this->fileEntryMap.insert(make_pair(pos->first, pos->second));
	}
}

/************************************************************************/
/* 现在使用的构造函数                                                                     */
/************************************************************************/
InvertedEntry::InvertedEntry(const string& term, const string& fileName, const int classifiedType)
{
	this->term = term;
	this->classifiedType = classifiedType;
	this->totalFreq = 1;/*创建时就表示有出现这个词的一项了*/
	
	/*创建副文件的项*/
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
	{// 手动拷贝啊T_T,以后要用copy
		this->fileEntryMap.insert(make_pair(pos->first, pos->second));
	}
	return *this;
}

/************************************************************************/
/* 得到这个词在这一类中出现的文件数                                                                     */
/************************************************************************/
unsigned int InvertedEntry::getDocNum() const
{
	/*可以简单的和副文件中与这个类对应的项数来表示
	* 因为副文件的每一项就表示一个文件
	*/
	return fileEntryMap.size();
}

/************************************************************************/
/* 增加这个词在这一类中出现的频率                                                                     */
/************************************************************************/
void InvertedEntry::addFreq(const string& fileName)
{
	totalFreq++;

	map<string, FileEntry>::iterator pos = fileEntryMap.find(fileName);
	if(pos == fileEntryMap.end())
	{// 如果在副文件中没有相应的一项，就添加这一项
		FileEntry fileEntry(fileName);
		fileEntryMap.insert(make_pair(fileName, fileEntry));
	}
	else
	{// 如果副文件中已经存在了这一项，就简单对那项频率加一
		pos->second.addFreq();
	}
}
