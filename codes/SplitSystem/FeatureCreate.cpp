#include "featurecreate.h"
#include "help.h"

#include <vector>
#include <deque>
#include <queue>
#include <utility>
#include <algorithm>
#include <cstdio>
#include <cmath>
using namespace std;

FeatureCreate::FeatureCreate(void)
{
}

FeatureCreate::~FeatureCreate(void)
{
}

double FeatureCreate::calcWeight(const string& term, const InvertedCreate& ic)
{
	return log10((double)ic.getTotalDocNum()/(double)ic.getTermDocNum(term));
}

/************************************************************************/
/* Use mutual information to pick features
/* Every class will have the same number of features
/*
/************************************************************************/
const map<string, Feature>& FeatureCreate::createFeatureMap(const map<string, InvertedEntry>& invertedMap, const InvertedCreate& ic)
{
	printf("total size of terms(added with all classes, maybe some same terms): %d\n", invertedMap.size());
	// total number of term frequency of all classes, key is the term, value is the frequency
	map<string, unsigned> freqMap;

	map<string, unsigned>::iterator mi;
	map<string, InvertedEntry>::const_iterator it;
	int i;

	// terms and their values for each class
	priority_queue<pair<double, string>, deque<pair<double, string> > >* pqs = new priority_queue<pair<double, string>, deque<pair<double, string> > >[Help::classNum];
	
	for (it=invertedMap.begin(); it!=invertedMap.end(); it++)
	{
		if ((mi = freqMap.find(it->second.term)) != freqMap.end())
		{
			mi->second += it->second.totalFreq;
		}
		else
		{
			freqMap[it->second.term] = it->second.totalFreq;
		}
	}

	printf("total size of different terms in all classes: %d\n", freqMap.size());

	for (it=invertedMap.begin(); it!=invertedMap.end(); it++)
	{
		if ((mi = freqMap.find(it->second.term)) != freqMap.end())
		{
			pqs[it->second.classifiedType-1].push(make_pair((double)(it->second.totalFreq)/mi->second, it->second.term));
		}
	}

	/*************/
	printf("size of terms in each classes:\n");
	for (i=0; i<Help::classNum; i++)
	{
		printf("class %d: %d\n", i+1, pqs[i].size());
	}
	/*************/

	int n = 0;	// current number of picked feature
	int ci = 0;	// current index of class type array "nems"
	int nEm = Help::classNum;	// number of non-empty priority queue
	int* nems = new int[Help::classNum];
	for (i=0; i<Help::classNum; i++)
	{
		nems[i] = i;
	}

	Feature fea(0, "", 0.0, 0);
	while (n < Help::featureMaxNum && nEm > 0)
	{
		if (pqs[nems[ci]].empty())
		{
			if (ci < nEm-1)
			{
				int t = nems[ci];
				copy(&nems[ci+1], &nems[nEm], &nems[ci]);
			}
			else
			{
				ci = 0;
			}
			nEm--;
		}
		else
		{
			pair<double, string> p = pqs[nems[ci]].top();
			if (featureMap.find(p.second) == featureMap.end())
			{
				fea.id = n + 1;
				fea.term = p.second;
				fea.type = nems[ci]+1;
				fea.score = calcWeight(p.second, ic);
				//featureMap[p.second] = Feature(n+1, p.second, p.first, nems[ci]+1);
				featureMap[p.second] = fea;
				n++;	
				pqs[nems[ci]].pop();
				ci = (++ci) % nEm;
			}
			else
			{
				pqs[nems[ci]].pop();
			}
		}
	}

	delete[] nems;
	delete[] pqs;

	string fn = Help::getProperty("featureFileName")+"_"+Help::itos(Help::classNum)+"_"+Help::itos(Help::featureMaxNum)+".dat";
	if (fn.length() > 0)
	{
		printToFile(fn);
	}

	return featureMap;
}

void FeatureCreate::printToFile(const string& fn)
{
	FILE* fp = fopen(fn.c_str(), "w");
	if (fp == 0)
	{
		printf("Cannot open file %s.", fn.c_str());
	}

	for (map<string, Feature>::const_iterator it=featureMap.begin(); it!=featureMap.end(); it++)
	{
		fprintf(fp, "%d %s %f %d\n", it->second.id, it->second.term.c_str(), it->second.score, it->second.type);
	}

	fclose(fp);
}

/************************************************************************/
/* Print featureMap human readable into a file or stdout
/************************************************************************/
void FeatureCreate::print(FILE* fp)
{
	fprintf(fp, "key=value(id/term/score/type),\n");
	for (map<string, Feature>::const_iterator it=featureMap.begin(); it!=featureMap.end(); it++)
	{
		fprintf(fp, "%s=%d/%s/%f/%d\n", it->first.c_str(), it->second.id, it->second.term.c_str(), it->second.score, it->second.type);
	}
}

const map<string, Feature>& FeatureCreate::getFeatureMap()
{
	return featureMap;
}
