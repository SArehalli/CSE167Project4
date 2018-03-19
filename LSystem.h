#pragma once

#include <vector>
#include <map>
#include <glm\mat4x4.hpp>
#include <random>
#include "Transformation.h"

class LSystem : 
	public Node
{
public:
	LSystem(int n);
	~LSystem();

	void iterate();

	virtual void draw(glm::mat4 M);
	virtual void update();
private:
	std::string rep;
	std::map<char, std::vector<std::string>> rules;

	std::string apply(char);
};

