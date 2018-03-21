#pragma once
#include "LSystem.h"
#include "Model.h"
#include "Tree.h"
#include <glm/mat4x4.hpp>
#include <stack>

#define MAZE_WIDTH 32 

class Maze :
   public LSystem
{
public:
   Maze(int n, GLint shader, GLint treeShader);
   ~Maze();
   void regenerate();

private:
   int n;
   GLint shader, treeShader;
   int maze[MAZE_WIDTH][MAZE_WIDTH];

   void build();
};

