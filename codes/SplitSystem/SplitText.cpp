#include "SplitText.h"
#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include <set>
#define BUFSIZE 1024 * 1024

const string SplitText::stopWordIndex[STOPWORDNUM] = {
	"/a", "/an", "/b", "/f", "/h",
	"/i", "/j", "/k", "/l", "/Ng",
	"/n", "/nr", "/ns", "/nt", "/nz",
	"/nx", "/s", "/vg", "/vd", "/vn",
	"/z", "/un", "/v", 
};

SplitText::SplitText(const string& componentPath)
{
	const string s = componentPath + "CSW.dll";
	this->componentPath = componentPath;
	hDll = LoadLibrary(s.c_str());
	
	int i;
	for(i = 0; i < STOPWORDNUM; i++)
	{
		stopWordSet.insert(stopWordIndex[i]);
	}
}

SplitText::~SplitText()
{
	FreeLibrary(hDll);
}

void SplitText::getSplitedText(const string& fileName, list<string>& wordList)
{
	string splitedText = getSplitedWord(fileName, 1);// 现在得到主要是得到含有词性的分词
	removeStopWord(splitedText, wordList);
}

string SplitText::getSplitedWord(const string& fileName, int type)
{
	char* readBuf = new char[BUFSIZE];	
	string sourceText, splitedText;
	FILE* textFile;
	lpSplitFun splitFun;

	//step1: read the file
	textFile = fopen(fileName.c_str(), "r");
	if(textFile == NULL)
	{// open the file failed
		return "";
	}

	size_t n;
	while((n = fread(readBuf, sizeof(char), BUFSIZE, textFile)) > 0)
	{
		readBuf[n] = '\0';
		sourceText.append(readBuf);  
	}
	fclose(textFile);
	delete[] readBuf; 

	//step2: split the source text
	splitFun = (lpSplitFun)GetProcAddress(hDll,"Split");
	if(splitFun!=NULL)
	{	
		char *text = strToNonConstchar(sourceText);
		char *path = strToNonConstchar(this->componentPath);
		// the real split the text 
		char *s = splitFun(text, type, path);
		splitedText.append(s);

		delete[] text;
		delete[] path;
	}


	return splitedText;
}
char* SplitText::strToNonConstchar(const string& sourceText)
{
	char* convert = new char[sourceText.length() + 1];
	memcpy(convert, sourceText.c_str(), sourceText.length());
	convert[sourceText.length()] = '\0';
	return convert;
}

void SplitText::removeStopWord(const string& sourceText, list<string>& wordList)
{
	const string delims(" \t\n");
	string::size_type wordBegIdx, wordEndIdx;

	wordBegIdx = sourceText.find_first_not_of(delims);
	while(wordBegIdx != string::npos)
	{
		string::size_type characBegIdx, characEndIdx;
		string word, character;

		wordEndIdx = sourceText.find_first_of(delims, wordBegIdx);
		characBegIdx = sourceText.find_first_not_of(delims, wordEndIdx);
		characEndIdx = sourceText.find_first_of(delims, characBegIdx);

		if(wordEndIdx == string::npos || characBegIdx == string::npos
			|| characEndIdx == string::npos)
		{
			std::cout << "removeStopWord:not a common splited sourceText!" << std::endl;
			return;
		}

		word = sourceText.substr(wordBegIdx, wordEndIdx - wordBegIdx);
		character = sourceText.substr(characBegIdx, characEndIdx - characBegIdx);
		if(stopWordSet.find(character) != stopWordSet.end())
		{
			wordList.push_front(word);
		}

		wordBegIdx = sourceText.find_first_not_of(delims, characEndIdx);
	}
}