#pragma once

#include <string>
using namespace std;

/************************************************************************/
/*��ÿһ��ѵ�����߲��Ե��ļ����е�ÿһ��term����������һ�����ݽṹ������Ӧ��
*������ݽṹ������¼���term������ļ��г��ֵ�Ƶ��
*�����Ҫ��InvertedEntry��һ��ʹ�þ�����˵����ļ���������ʾ���ļ��е�һ��
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
	string fileName;		/*������ݽṹ����Ӧ���ļ����ļ���*/
	unsigned int freq;		/*��Ӧ��term��fileName���ֵ�Ƶ��*/
};
