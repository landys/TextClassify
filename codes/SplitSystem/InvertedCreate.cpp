#include "InvertedCreate.h"
#include "Help.h"
#include "windows.h"
#include "stdlib.h"

/************************************************************************/
/* ��Ĺ��캯��                                                                     */
/************************************************************************/
InvertedCreate::InvertedCreate(const string& componentPath):
	splitText(componentPath)
{
	this->totalDocNum = 0;
	this->isCreate = false;
}

InvertedCreate::~InvertedCreate(void)
{
}

/************************************************************************/
/* ������������������������ļ�
@param trainSet ѵ�����ļ����ڵ�Ŀ¼
@return ���������������map
*/
/************************************************************************/
const map<string, InvertedEntry>& InvertedCreate::createInvertedFile(const map<int, string>& trainSet)
{
	int type;
	map<int, string>::const_iterator configPos;
	int i;
	
	/*step 1: �������ԭ�ȵĵ����ļ�*/
	invertedMap.clear();
	/*step 2: ��������ѵ��Ŀ¼*/
	for(configPos = trainSet.begin(); configPos != trainSet.end(); configPos++)
	{
		type = configPos->first;
		list<string> fileList = Help::getFileNameList(configPos->second);/*�õ�һ��Ŀ¼�µ������ļ���*/
		/*��������������*/
		createInvertedEntry(fileList, type);
		/*���ӱ����ļ���*/
		this->totalDocNum += fileList.size();
	}
	
	this->isCreate = true;
	showResult();
	return invertedMap;
}

/************************************************************************/
/* �������������ѵ���ļ��б�ĵ���������
*@param fileList	Ҫѵ�����ļ��б�
*@param type ������ļ��б����������
*/
/************************************************************************/
void InvertedCreate::createInvertedEntry(const list<string>& fileList, int type)
{
	list<string>::const_iterator pos;

	for(pos = fileList.begin(); pos != fileList.end(); pos++)
	{// �����ļ��б�
		printf("%s,", (*pos).c_str());
		/*��ѵ���ļ����зִ�*/
		list<string> wordList;
		splitText.getSplitedText(*pos, wordList);
		/*���뵹��������*/
		insertWordsToInvertedFile(wordList, *pos, type);
	}
	printf("\n");
}

/************************************************************************/
/* ���������б���������������
*@param wordList	������б�
*@param fileName	��Щ������б�
*@param type		��Щ�������������
*/
/************************************************************************/
void InvertedCreate::insertWordsToInvertedFile(const list<string>& wordList, const string& fileName, const int type)
{
	list<string>::const_iterator pos;

	for(pos = wordList.begin(); pos != wordList.end(); pos++)
	{	
		/*������뵹��������*/
		insertWord(*pos, fileName, type);
	}
}

/************************************************************************/
/*����һ������������                                                                      */
/************************************************************************/
void InvertedCreate::insertWord(const string& word, const string& fileName, const int type)
{
	string key = getMapKey(word, type);
	map<string, InvertedEntry>::iterator pos;
	InvertedEntry f();

	pos = invertedMap.find(key);
	if(pos != invertedMap.end())
	{// ��map���Ѿ�������һ��
		pos->second.addFreq(fileName);
	}
	else
	{// ��map�в�������һ�Ҫ��map�������һ��
		addInvertedEntry(word, fileName, type);
	}
}

string InvertedCreate::getMapKey(const string& word, const int type) const
{
	string key = word + "||";
	
	key += type;
	return key;
}

void InvertedCreate::addInvertedEntry(const string& word, const string& fileName, const int type)
{
	InvertedEntry invertedEntry(word, fileName, type);

	invertedMap.insert(make_pair(getMapKey(word, type), invertedEntry));
}

unsigned int InvertedCreate::getTotalDocNum() const
{
	if(isCreate == true)
	{
		return totalDocNum;
	}
	else
	{
		return 0;
	}
}

/************************************************************************/
/* ��ӡ���Խ������Ҫ��������ļ�                                                                    */
/************************************************************************/
void InvertedCreate::showResult() const
{
	FILE *invertFile1, *invertFile2;
	string fileName1 = Help::getProperty("invertedMainFile");
	string fileName2 = Help::getProperty("invertedAuxFile");

	if((invertFile1 = fopen(fileName1.c_str(), "w")) == NULL)
	{
		printf("can not open file %s.\n", fileName1.c_str());
	}

	if((invertFile2 = fopen(fileName2.c_str(), "w")) == NULL)
	{
		printf("can not open file %s.\n", fileName2.c_str());
	}
	fprintf(invertFile1, "term                type                   freq              docNum\n");
	fprintf(invertFile2, "term                   fileName                     freq\n");
	map<string, InvertedEntry>::const_iterator pos;
	for(pos = invertedMap.begin(); pos != invertedMap.end(); pos++)
	{
		const InvertedEntry& inverted = pos->second;
		
		fprintf(invertFile1, "%-20s  %-20d  %-10u %-10d\n", inverted.term.c_str(), inverted.classifiedType, inverted.totalFreq, inverted.getDocNum());
		map<string, FileEntry>::const_iterator filePos;
		for(filePos = inverted.fileEntryMap.begin(); filePos != inverted.fileEntryMap.end(); filePos++)
		{
			const FileEntry& fileEntry = filePos->second;
			fprintf(invertFile2, "%-20s  %-30s  %-6u\n", inverted.term.c_str(), fileEntry.fileName.c_str(), fileEntry.freq);
		}
	}
	fclose(invertFile1);
	fclose(invertFile2);
}

/************************************************************************/
/* �õ�һ�����ڸ������г��ֵ����ļ���                                                                     */
/************************************************************************/
unsigned int InvertedCreate::getTermDocNum(const string& term) const
{
	int i;
	int docNum = 0;

	for(i = 1; i <= Help::classNum; i++)
	{
		string key = getMapKey(term, i);
		map<string, InvertedEntry>::const_iterator pos = invertedMap.find(key);
		if(pos != invertedMap.end())
		{
			docNum += pos->second.getDocNum();
		}
	}

	return docNum;
}