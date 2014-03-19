#include "FileFeatureProc.h"
#include "Help.h"

int main(int argc, char* argv[])
{
	Help::initApp("..\\sample\\properties.dat");
	Help::print();

	if(argc > 1)
	{
		if(argv[1][0] == '1')
		{
			printf("start extractFeatureVector.................\n");
			FileFeatureProc ffp(Help::getProperty("splitComponent"));
			ffp.extractFeatureVector(Help::testSet);
			ffp.computeFilesFeature(Help::testSet);
		}
		else if(argv[1][0] == '2')
		{
			printf("start computeTestFilesFeature...............\n");
			FileFeatureProc ffp(Help::getProperty("splitComponent"));
			ffp.computeFilesFeature(Help::testSet);
		}
	}
	return 0;
}
