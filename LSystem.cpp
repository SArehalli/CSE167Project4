#include "LSystem.h"

LSystem::LSystem(int n) :
	Transformation(glm::mat4(1.0f))
{
	rep = "X";
}


LSystem::~LSystem()
{
}

void LSystem::iterate()
{
	std::string out = "";
	for (int i = 0; i < rep.size(); i++)
	{
		out.append(apply(rep[i]));
	}
	rep = out;
}

void LSystem::addRule(char c, std::string s)
{
	rules[c].push_back(s);
}

void LSystem::update()
{
}

std::string LSystem::apply(char c)
{
	std::vector<std::string> options = rules[c];
	if (options.size() > 0)
	{
		int idx = rand() % options.size();
		return options[idx];
	}
	else
	{
		return std::string(1, c);
	}
}
