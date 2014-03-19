#pragma once

#include <string>

using std::string;

class Feature
{
public:
	Feature(void);
	virtual ~Feature(void);
	Feature(int id, const string& term, double score, int type);
	Feature(const Feature& feature);
	Feature& operator=(const Feature& feature);
public:
	int id;	// number, 1, 2, 3, ...
	string term;	// word of feature
	double score;	// the score of the feature
	int type;	// class type
};
