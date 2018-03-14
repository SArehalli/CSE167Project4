#pragma once
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
class Skybox
{
public:
	Skybox(char ** imagePaths);
	~Skybox();
	
	void draw(GLuint shaderProgram);

private:
	glm::mat4 toWorld;
	unsigned int textureID;
	unsigned int VAO, VBO, EBO;
	unsigned char* loadPPM(const char* filename, int& width, int& height);
};

