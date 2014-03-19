#include "InvertedCreate.h"
#include "Help.h"
#include "windows.h"
#include "stdlib.h"

/************************************************************************/
/* 类的构造函数                                                                     */
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
/* 这个函数用来创建倒排索引文件
@param trainSet 训练集文件所在的目录
@return 包含倒排索引项的map
*/
/************************************************************************/
const map<string, InvertedEntry>& InvertedCreate::createInvertedFile(const map<int, string>& trainSet)
{
	int type;
	map<int, string>::const_iterator configPos;
	int i;
	
	/*step 1: 首先清空原先的倒排文件*/
	invertedMap.clear();
	/*step 2: 遍历所有训练目录*/
	for(configPos = trainSet.begin(); configPos != trainSet.end(); configPos++)
	{
		type = configPos->first;
		list<string> fileList = Help::getFileNameList(configPos->second);/*得到一个目录下的所有文件名*/
		/*创建倒排索引项*/
		createInvertedEntry(fileList, type);
		/*增加遍历文件数*/
		this->totalDocNum += fileList.size();
	}
	
	this->isCreate = true;
	showResult();
	return invertedMap;
}

/************************************************************************/
/* 创建传入的所有训练文件列表的倒排索引项
*@param fileList	要训练的文件列表
*@param type 传入的文件列表所属的类别
*/
/************************************************************************/
void InvertedCreate::createInvertedEntry(const list<string>& fileList, int type)
{
	list<string>::const_iterator pos;

	for(pos = fileList.begin(); pos != fileList.end(); pos++)
	{// 遍历文件列表
		printf("%s,", (*pos).c_str());
		/*对训练文件进行分词*/
		list<string> wordList;
		splitText.getSplitedText(*pos, wordList);
		/*插入倒排索引项*/
		insertWordsToInvertedFile(wordList, *pos, type);
	}
	printf("\n");
}

/************************************************************************/
/* 遍历词语列表，产生倒排索引项
*@param wordList	词语的列表
*@param fileName	这些词语的列表
*@param type		这些词语所属的类别
*/
/************************************************************************/
void InvertedCreate::insertWordsToInvertedFile(const list<string>& wordList, const string& fileName, const int type)
{
	list<string>::const_iterator pos;

	for(pos = wordList.begin(); pos != wordList.end(); pos++)
	{	
		/*逐个插入倒排索引项*/
		insertWord(*pos, fileName, type);
	}
}

/************************************************************************/
/*插入一个倒排索引项                                                                      */
/************************************************************************/
void InvertedCreate::insertWord(const string& word, const string& fileName, const int type)
{
	string key = getMapKey(word, type);
	map<string, InvertedEntry>::iterator pos;
	InvertedEntry f();

	pos = invertedMap.find(key);
	if(pos != invertedMap.end())
	{// 在map中已经存在这一项
		pos->second.addFreq(fileName);
	}
	else
	{// 在map中不存在这一项，要向map中添加这一项
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
/* 打印测试结果，主要输出倒排文件                                                                    */
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
/* 得到一个词在各个类中出现的总文件数                                                                     */
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