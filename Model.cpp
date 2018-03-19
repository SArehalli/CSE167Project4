#include "Model.h"
#include "Window.h"


Model::Model(char * filename, GLuint shaderProgram)
{
	loadModel(filename);

	this->shaderProgram = shaderProgram;
}

Model::~Model()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO[0]);
	glDeleteBuffers(1, &VBO[1]);
	glDeleteBuffers(1, &VBO[2]);
	glDeleteBuffers(1, &EBO);
}

void Model::loadModel(char * filepath)
{
	// Parse the OBJ file
	float x, y, z, r, g, b;
	int v1, v2, v3, n1, n2, n3;
	char type[3];
	char c;

	FILE* fd = fopen(filepath, "r");
	if (!fd) {
		fprintf(stderr, "Error: Failed to load object at %s\n", filepath);
		exit(-1);
	}

	while (fscanf(fd, "%2s ", &type) > 0) {
		if (strcmp(type, "vn") == 0) {
			fscanf(fd, "%f %f %f", &x, &y, &z);
			normals.push_back(glm::normalize(glm::vec3(x, y, z)));
		}
		else if (strcmp(type, "v") == 0) {
			int count = fscanf(fd, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			vertices.push_back(glm::vec3(x, y, z));
			if (count <= 3) r = g = b = 1.0f;
			colors.push_back(glm::vec3(r, g, b));
		}
		else if (strcmp(type, "f") == 0) {
			fscanf(fd, "%d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
			indices.push_back(v1-1);
			indices.push_back(v2-1);
			indices.push_back(v3-1);
		}
		else if (type[0] == '#') {
			while ((c = fgetc(fd)) != '\n' && c != EOF) {};
		}
	}
	fprintf(stderr, "%s:\n", filepath);
	normalize();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, &VBO[0]);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), colors.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *) 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::normalize()
{
	// Centering
	glm::vec3 min = glm::vec3(FLT_MAX);
	glm::vec3 max = glm::vec3(-FLT_MAX);
	for (unsigned int j = 0; j < vertices.size(); j++ ) {
		unsigned int i = j;
		min.x = vertices[i].x < min.x ? vertices[i].x : min.x;
		min.y = vertices[i].y < min.y ? vertices[i].y : min.y;
		min.z = vertices[i].z < min.z ? vertices[i].z : min.z;
		max.x = vertices[i].x > max.x ? vertices[i].x : max.x;
		max.y = vertices[i].y > max.y ? vertices[i].y : max.y;
		max.z = vertices[i].z > max.z ? vertices[i].z : max.z;
	}
	glm::vec3 center = 0.5f * (min + max);

	// Scaling
	glm::vec3 scale = max - center;
	float scale_max = 0.0f;
	scale_max = scale.x > scale_max ? scale.x : scale_max;
	scale_max = scale.y > scale_max ? scale.y : scale_max; 
	scale_max = scale.z > scale_max ? scale.z : scale_max;
	scale_max = scale_max > 0.0f ? 1.0f / scale_max : 1.0f;
	this->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(scale_max)) * glm::translate(glm::mat4(1.0f), -center);
}

void Model::draw(glm::mat4 M) {
	GLint uProjection, uModel, uView;
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uView = glGetUniformLocation(shaderProgram, "view");
	uModel = glGetUniformLocation(shaderProgram, "model");

	glm::mat4 model = M * toWorld;

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &model[0][0]);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Model::update()
{
}
