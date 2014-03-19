#pragma once

#include "FileEntry.h"
#include <string>
#include <map>
using namespace std;

/************************************************************************/
/* 这个类表示倒排文件的主文件中的一项，现在倒排文件的格式为：
* 主文件：term(词语), type(类别)， freq(类别出现的总频率)， docNum(出现的文件数)
* 副文件：term(词语), fileName(出现的文件名)，freq(在文件中出现的频率)
* 它代表这某个词语在某种类别的文件集合中出现总频率
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
	string term;		/*类代表的词语*/
	int classifiedType;	/*代表这个词语是从哪类文件中提取出来的*/
	unsigned int totalFreq;		/*这个词语在这类文件中出现的总频率*/
	map<string, FileEntry> fileEntryMap;	/*与这项对应的副文件中的项*/

private:
	void addFileEntry(const string& fileName);
};
