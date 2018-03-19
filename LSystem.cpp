#include "LSystem.h"

LSystem::LSystem(int n)
{
	for (int i = 0; i < n; i++) iterate();
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

void LSystem::draw(glm::mat4 M)
{
}

void LSystem::update()
{
}

std::string LSystem::apply(char c)
{
	std::vector<std::string> options = rules[c];
	int idx = (int)(rand() * options.size());

	return options[idx];
}
