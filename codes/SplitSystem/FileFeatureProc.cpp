#include "FileFeatureProc.h"
#include "Help.h"
#include "math.h"

FileFeatureProc::FileFeatureProc(const string& componentPath):
	splitText(componentPath), invertedCreate(componentPath)
{
}

FileFeatureProc::~FileFeatureProc(void)
{
}

void FileFeatureProc::computeFilesFeature(const map<int, string>& configSet)
{
	FILE *textVectorFile, *textNameFile;
	string suffix = "_" + Help::itos(Help::classNum) + "_" + Help::itos(Help::featureMaxNum) + ".txt";
	string textVectorFileName = Help::getProperty("textVectorFile") + suffix;
	string textNameFileName = Help::getProperty("textNameFile") + suffix;
	double* featureRes = new double[Help::featureMaxNum + 1];

	if((textVectorFile = fopen(textVectorFileName.c_str(), "w")) == NULL || (textNameFile = fopen(textNameFileName.c_str(), "w")) == NULL)
	{
		printf("Can not create file %s or %s.\n", textVectorFileName.c_str(), textNameFileName.c_str());
	}

	map<int, string>::const_iterator configPos;
	for(configPos = configSet.begin(); configPos != configSet.end(); configPos++)
	{// 遍历配置列表
		int type = configPos->first;
		const string& inputDir = configPos->second;
		const list<string> inputFileList = Help::getFileNameList(inputDir);
		list<string>::const_iterator filePos;
		
		for(filePos = inputFileList.begin(); filePos != inputFileList.end(); filePos++)
		{
			printf("%s,", (*filePos).c_str());
			computeOneFilesFeature(featureRes, *filePos);
			saveFilesFeature(textVectorFile, textNameFile, featureRes, type, *filePos);
		}
	}
	printf("\n");
	fclose(textNameFile);
	fclose(textVectorFile);
	delete[] featureRes;
}

void FileFeatureProc::computeOneFilesFeature(double* featureRes, const string& fileName)
{
	fill(featureRes, featureRes + (Help::featureMaxNum + 1), 0.0);
	// 得到分词列表
	list<string> wordList;
	splitText.getSplitedText(fileName, wordList);
	// 得到特征
	string s = Help::getProperty("featureFileName")+"_"+Help::itos(Help::classNum)+"_"+Help::itos(Help::featureMaxNum)+".dat";
	const map<string, Feature>& featureMap = Help::loadFeatureMap(s);
	// 得到词频
	map<string, unsigned int> wordFreqMap;
	getWordsFreqMap(wordList, wordFreqMap);

	list<string>::const_iterator wordListPos;
	for(wordListPos = wordList.begin(); wordListPos != wordList.end(); wordListPos++)
	{// we scan the splited word list
		map<string, Feature>::const_iterator featurePos = featureMap.find(*wordListPos);
		if(featurePos != featureMap.end())
		{// if the word is in the features
			int wordId = featurePos->second.id;	
			if(fabs(featureRes[wordId]) > 0.00000001)
			{// we have computed this feature dimensionality
				continue;
			}
			else
			{
				map<string, unsigned int>::const_iterator freqPos = wordFreqMap.find(*wordListPos);
				unsigned int freq;
				if(freqPos != wordFreqMap.end())
				{
					freq = freqPos->second;
				}
				else
				{// I think it would not happen
					continue;
				}
				// get the tf*idf value
				featureRes[wordId] = computeWeight(freq, featurePos->second.score);
			}
		}
	}
}

void FileFeatureProc::getWordsFreqMap(const list<string>& wordList, map<string, unsigned int>& wordFreqMap)
{
	list<string>::const_iterator listPos;

	for(listPos = wordList.begin(); listPos != wordList.end(); listPos++)
	{
		map<string, unsigned int>::iterator mapPos = wordFreqMap.find(*listPos);
		if(mapPos == wordFreqMap.end())
		{
			wordFreqMap.insert(make_pair(*listPos, 1));
		}
		else
		{
			mapPos->second++;
		}
	}
}

double FileFeatureProc::computeWeight(unsigned int freq, double score)
{// computer file feature by tf*idt 
	double TFIDF = freq * score;
	return TFIDF;
}

const map<string, Feature>& FileFeatureProc::extractFeatureVector(const map<int, string>& testSet)
{
	const map<string, InvertedEntry>& invertedMap = invertedCreate.createInvertedFile(testSet);	
	return featureCreate.createFeatureMap(invertedMap, invertedCreate);
}

void FileFeatureProc::saveFilesFeature(FILE* saveFile, FILE* nameFile, const double* featureRes, const int type, const string& fileName)
{
	int i;

	fprintf(nameFile, "%d %s\n", type, fileName.c_str());
	
	fprintf(saveFile, "%d ", type);
	for(i = 1; i <= Help::featureMaxNum; i++)
	{
		if(fabs(featureRes[i]) > 0.00000001)
		{
			fprintf(saveFile, "%d:%f ", i, featureRes[i]);
		}
	}
	fprintf(saveFile, "\n");
}