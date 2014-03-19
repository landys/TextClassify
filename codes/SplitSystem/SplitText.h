#pragma once

#include <string>
#include <conio.h>
#include <list>
#include <set>
#include "windows.h"
using namespace std;

typedef char* (* lpSplitFun)(char*, long, char*);

#define STOPWORDNUM 24

class SplitText
{
public:
	SplitText(const string& componentPath);
	~SplitText();
	void getSplitedText(const string& fileName, list<string>& wordList);
private:
	string getSplitedWord(const string& fileName, int type);
	void removeStopWord(const string& sourceText, list<string>& wordList);
	char* strToNonConstchar(const string& sourceText);
private:
	string componentPath;
	HINSTANCE hDll;
	// stopword
	set<string> stopWordSet;
	static const string stopWordIndex[STOPWORDNUM];
};
