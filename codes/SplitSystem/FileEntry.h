#pragma once

#include <string>
using namespace std;

/************************************************************************/
/*在每一个训练或者测试的文件的中的每一个term都有这样的一个数据结构跟他对应，
*这个数据结构用来记录这个term在这个文件中出现的频率
*这个类要与InvertedEntry类一起使用就组成了倒排文件，这个类表示副文件中的一项
*/
/************************************************************************/
class FileEntry
{
public:
	FileEntry(const string& fileName);
	FileEntry(const FileEntry& fileEntry);
	FileEntry& operator=(const FileEntry& copy);
	~FileEntry(void);
	void addFreq();

public:
	string fileName;		/*这个数据结构所对应的文件的文件名*/
	unsigned int freq;		/*对应的term在fileName出现的频率*/
};
