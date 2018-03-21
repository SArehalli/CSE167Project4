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
	Tree(int n, Shader *shader);
	void regenerate();
	~Tree();
	
private:
	void build(int n);
	Shader *shader;
	int n;
};

#endif // !TREE
