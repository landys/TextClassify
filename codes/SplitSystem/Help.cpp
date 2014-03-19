#include "Help.h"
#include <algorithm>
#include <deque>
#include "windows.h"

int Help::classNum = 0;
int Help::featureMaxNum = 0;
bool Help::isLoad = false;
map<string, string> Help::properties;
map<int, string> Help::testSet;
map<string, Feature> Help::featureMap;

Help::Help(void)
{
}

Help::~Help(void)
{
}

list<string> Help::getFileNameList(const string& dir)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	string fileParser = dir + "*.txt";
	list<string> fileNameList;
	DWORD dwError;

	hFind = FindFirstFile(fileParser.c_str(), &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		printf ("Invalid file handle. Error is %u\n", GetLastError());
		return fileNameList;
	}
	else
	{
		fileNameList.push_back(dir + FindFileData.cFileName);
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			fileNameList.push_back(dir + FindFileData.cFileName);
		}

		dwError = GetLastError();
		FindClose(hFind);
		if (dwError != ERROR_NO_MORE_FILES) 
		{
			printf ("FindNextFile error. Error is %u\n", dwError);
			return fileNameList;
		}
	}

	return fileNameList;
}

/************************************************************************/
/* "": 0, "1234": 1234, "-1234": -1234
/************************************************************************/
int Help::stoi(const string& s)
{
	unsigned n = s.length();
	if (n == 0)
	{
		return 0;
	}
	int f = 0;
	int i = 0;
	if (s[0] == '-')
	{
		f = 1;
		i++;
	}
	int r = 0;
	for (; i<n; i++)
	{
		r = r * 10 + (s[i]-'0');
	}
	return f==1 ? -r : r;
}

/************************************************************************/
/* integer to string, support negative integer
/************************************************************************/
string Help::itos(int m)
{
	char c[15];
	deque<char> de;

	fill(&c[0], &c[15], '\0');

	int i = 0;
	if (m < 0)
	{
		c[0] = '-';
		i++;
		m = -m;
	}

	while (m / 10 != 0)
	{
		de.push_back(m % 10 + '0');
		m /= 10;
	}
	de.push_back(m + '0');

	copy(de.rbegin(), de.rend(), &c[i]);

	return c;
}


/************************************************************************/
/* read config file to set some parameters
/************************************************************************/
void Help::initApp(const string& fn)
{
	FILE* fp = fopen(fn.c_str(), "r");
	if (fp == 0)
	{
		printf("Cannnot open file %s.", fn.c_str());
	}

	char key[100];
	char value[256];
	char temp[256];
	while (fscanf(fp, "%[^=]=%s", key, value) != -1)
	{
		// read \n
		fgets(temp, 255, fp);
		properties[key] = value;
		if (key[0] == '0')
		{
			testSet[stoi(&key[1])] = value;
		}
	}
	fclose(fp);

	map<string, string>::iterator it = properties.find("featureMaxNum");
	if (it != properties.end())
	{
		featureMaxNum = stoi(it->second);
	}

	it = properties.find("classNum");
	if (it != properties.end())
	{
		classNum = stoi(it->second);
	}
}

string Help::getProperty(const string& key)
{
	map<string, string>::iterator it = properties.find(key);
	if (it != properties.end())
	{
		return it->second;
	}
	else
	{
		printf("No such property %s.", key.c_str());
		return "";
	}
}

const map<string, Feature>& Help::loadFeatureMap(const string& fn)
{
	if (isLoad)
	{
		return featureMap;
	}

	FILE* fp = fopen(fn.c_str(), "r");
	if (fp == 0)
	{
		printf("Cannot open file %s.", fn.c_str());
	}

	Feature fea(0, "", 0.0, 0);
	char term[100];
	while (fscanf(fp, "%d %s %lf %d\n", &fea.id, term, &fea.score, &fea.type) != -1)
	{
		fea.term = term;
		featureMap[fea.term] = fea;
	}

	fclose(fp);

	isLoad = true;

	return featureMap;
}

void Help::printMapIntString(const map<int, string>& m)
{
	for (map<int, string>::const_iterator it=m.begin(); it!=m.end(); it++)
	{
		printf("%d=%s\n", it->first, it->second.c_str());
	}
}

void Help::printMapStringString(const map<string, string>& m)
{
	for (map<string, string>::const_iterator it=m.begin(); it!=m.end(); it++)
	{
		printf("%s=%s\n", it->first.c_str(), it->second.c_str());
	}
}

void Help::printMapStringFeature(const map<string, Feature>& m)
{
	printf("key=value(id term score type),\n");
	int n = 0;
	for (map<string, Feature>::const_iterator it=featureMap.begin(); it!=featureMap.end(); it++)
	{
		printf("%d: %s=%d %s %f %d\n", ++n, it->first.c_str(), it->second.id, it->second.term.c_str(), it->second.score, it->second.type);
	}
}

void Help::print()
{
	printf("************************************\n");
	printf("classNum=%d\n", Help::classNum);
	printf("featureMaxNum=%d\n", Help::featureMaxNum);
	printf("**************properties************\n");
	printMapStringString(properties);
	printf("***************testSet**************\n");
	printMapIntString(testSet);
	printf("************************************\n");
}
