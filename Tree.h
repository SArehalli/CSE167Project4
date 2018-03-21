#pragma once
#ifndef TREE_H
#define TREE_H

#include "LSystem.h"
#include "Model.h"
#include <stack>

class Tree :
	public LSystem
{
public:
	Tree(int n, GLint shader);
	void regenerate();
	~Tree();
	
private:
	void build();
	GLint shader;
	int n;
};

#endif // !TREE
