#pragma once
#include "SplitText.h"
#include "Feature.h"
#include "FeatureCreate.h"
#include "InvertedCreate.h"
#include "stdlib.h"
#include <string>
#include <list>
#include <map>
#include <algorithm>
using namespace std;

class FileFeatureProc
{
public:
	FileFeatureProc(const string& componentPath);
	virtual ~FileFeatureProc(void);
	void computeFilesFeature(const map<int, string>& configSet);
	const map<string, Feature>& extractFeatureVector(const map<int, string>& trainSet);
private:
	void saveFilesFeature(FILE* saveFile, FILE* nameFile, const double* featureRes, const int type, const string& fileName);
	void computeOneFilesFeature(double* featureRes, const string& fileName);
	void getWordsFreqMap(const list<string>& wordList, map<string, unsigned int>& wordFreqMap);
	double computeWeight(unsigned int freq, double score);
private:
	SplitText splitText;
	InvertedCreate invertedCreate;
	FeatureCreate featureCreate;
};
