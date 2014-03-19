#pragma once

#include "Feature.h"
#include <list>
#include <map>
#include <string>

using namespace std;

class Help
{
public:
	Help(void);
	~Help(void);

	static list<string> getFileNameList(const string& dir);

	// "": 0, "1234": 1234, "-1234": -1234
	static int stoi(const string& s);
	static string itos(int m);

	// read config file to set some parameters
	static void initApp(const string& fn);

	// load feature properties
	static const map<string, Feature>& loadFeatureMap(const string& fn);

	// get property
	static string getProperty(const string& key);

	static void printMapIntString(const map<int, string>& m);
	static void printMapStringString(const map<string, string>& m);
	static void printMapStringFeature(const map<string, Feature>& m);
	static void print();
public:
	static int classNum;
	static int featureMaxNum;
	static map<int, string> testSet;
private:
	static bool isLoad;
	static map<string, string> properties;
	static map<string, Feature> featureMap;
};
