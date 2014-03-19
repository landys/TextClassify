#include "feature.h"
#include "InvertedCreate.h"

Feature::Feature(void) : id(-1), term(""), score(0.0), type(0)
{
}

Feature::~Feature(void)
{
}

Feature::Feature(int id, const string& term, double score, int type) : id(id), term(term), score(score), type(type)
{
}

Feature::Feature(const Feature& feature) : id(feature.id), term(feature.term), score(feature.score), type(feature.type)
{
}

Feature& Feature::operator=(const Feature& feature)
{
	id = feature.id;
	term = feature.term;
	score = feature.score;
	type = feature.type;

	return *this;
}
