#pragma once
#include "LSystem.h"
#include "Model.h"
#include <glm/mat4x4.hpp>
#include <stack>

#define MAZE_WIDTH 48 

class Maze :
	public LSystem
{
public:
	Maze(int n, GLint shader);
	~Maze();
	void regenerate();

private:
	int n;
	GLint shader;
	bool maze[MAZE_WIDTH][MAZE_WIDTH];

	void build();
};

