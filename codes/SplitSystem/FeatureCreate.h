#pragma once

#include "Feature.h"
#include "InvertedCreate.h"

class FeatureCreate
{
public:
	FeatureCreate(void);
	virtual ~FeatureCreate(void);

	double calcWeight(const string& term, const InvertedCreate& ic);
	const map<string, Feature>& createFeatureMap(const map<string, InvertedEntry>& invertedMap, const InvertedCreate& ic);
	const map<string, Feature>& getFeatureMap();
	void printToFile(const string& fn);
	void print(FILE* fp);
private:
	map<string, Feature> featureMap;
};
