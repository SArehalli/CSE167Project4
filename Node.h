#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


class Node
{
public:
	Node();
	virtual void draw(glm::mat4 M);
	virtual void update();
	virtual void regenerate();
   virtual void Draw(glm::mat4 M, GLint shader);
   virtual void DrawWBB(GLint shaderOBJ, GLint shaderBB, glm::mat4 obj2World);
	~Node();
};

