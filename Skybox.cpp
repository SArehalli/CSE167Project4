#include "Skybox.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>   // include math functions, such as sin, cos, M_PI
#include <iostream> // allow c++ style console printouts
#include "Window.h"

using namespace std; // allow console printouts without std::


Skybox::Skybox(char ** imagePaths)
{
	// Load Textures
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


	for (int i = 0; i < 6; i++) 
	{
		int tWidth, tHeight;
		unsigned char *tData = loadPPM(imagePaths[i], tWidth, tHeight);
		if (!tData)
		{
			cerr << "Could not load texture at " << imagePaths[i] << "!";
			exit(-1);
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Vertices
	float vertices[] = {
		-1.0f, -1.0f, -1.0f, // 0
		-1.0f, -1.0f,  1.0f, // 1
		-1.0f,  1.0f,  1.0f, // 2
		-1.0f,  1.0f, -1.0f, // 3
		 1.0f,  1.0f,  1.0f, // 4
		 1.0f, -1.0f,  1.0f, // 5
		 1.0f, -1.0f, -1.0f, // 6
		 1.0f,  1.0f, -1.0f  // 7
	};

	unsigned int indices[] = {
		1, 2, 4,
		4, 5, 1,

		3, 0, 6,
		6, 7, 3,

		1, 0, 3,
		3, 2, 1,

		6, 5, 4,
		4, 7, 6,

		3, 7, 4,
		4, 2, 3,

		5, 1, 0,
		0, 6, 5
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 12 * 3, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//
	toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(300.0f));
}


Skybox::~Skybox()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Skybox::draw(GLuint shaderProgram)
{
	GLuint uProjection = glGetUniformLocation(shaderProgram, "projection");
	GLuint uView = glGetUniformLocation(shaderProgram, "view");
	GLuint uModel = glGetUniformLocation(shaderProgram, "model");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glDrawElements(GL_TRIANGLES, 3 * 12, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}


/** Load a ppm file from disk.
@input filename The location of the PPM file.  If the file is not found, an error message
will be printed and this function will return 0
@input width This will be modified to contain the width of the loaded image, or 0 if file not found
@input height This will be modified to contain the height of the loaded image, or 0 if file not found
@return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
unsigned char* Skybox::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}