#pragma once

#include <vector>
#include <map>
#include <glm\mat4x4.hpp>
#include <random>
#include "Transformation.h"

class LSystem : 
	public Transformation
{
public:
	LSystem(int n);
	~LSystem();

	void iterate();
	void addRule(char c, std::string);

	virtual void update();
protected:
	std::string rep;
	std::map<char, std::vector<std::string>> rules;
	std::string apply(char);
};

