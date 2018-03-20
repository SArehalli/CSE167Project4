#include "Tree.h"



Tree::Tree(int n, Shader *shader) :
	LSystem(n)
{
	this->shader = shader;
	addRule('X', "F[-FX][+FX]");
	addRule('X', "F[^FX][&FX]");
	build(n);
}

Tree::~Tree()
{
}

void Tree::build(int n)
{
	for (int i = 0; i < n; i++) iterate();

	GLfloat angle = 25.0f/360.0f * glm::pi<float>();
	glm::vec3 dir = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

	GLfloat branch_len = 1.0f;
	Model *branch = new Model("../objs/cylinder2.obj", shader);
	Model *sphere = new Model("../objs/sphere.obj", shader);
	Transformation *branchResize = new Transformation(glm::mat4(1.0f));
	Transformation *scenePtr = branchResize;
	branchResize->addChild(branch);

	std::stack<glm::mat4> rotStack;
	std::stack<glm::mat4> movStack;

	Transformation *rotT, *movT;
	glm::mat4 rot, mov;

	for (int idx = 0; idx < rep.size(); idx++) {
		char c = rep[idx];
		movT = new Transformation(mov);
		switch (c) 
		{
		case 'F':
			movT = new Transformation(mov);
			rotT = new Transformation(rot);
			this->addChild(movT);
			movT->addChild(rotT);
			rotT->addChild(branchResize);
		case 'f':
			mov = glm::translate(glm::mat4(1.0f), dir * branch_len) * mov;
			break;

		case '[':
			rotStack.push(rot);
			movStack.push(mov);
			break;
		case ']':
			mov = movStack.top();
			rot = rotStack.top();
			movStack.pop();
			rotStack.pop();
			dir = glm::mat3(rot) * glm::vec3(0.0f, 1.0f, 0.0f);
			right = glm::mat3(rot) * glm::vec3(1.0f, 0.0f, 0.0f);
			break;

		case '+':
			rot = glm::rotate(glm::mat4(1.0f), angle, glm::cross(dir, right)) * rot;
			dir = glm::mat3(rot) * glm::vec3(0.0f, 1.0f, 0.0f);
			right = glm::mat3(rot) * glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case '-':
			rot = glm::rotate(glm::mat4(1.0f), -angle, glm::cross(dir, right)) * rot;
			dir = glm::mat3(rot) * glm::vec3(0.0f, 1.0f, 0.0f);
			right = glm::mat3(rot) * glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case '&':
			rot = glm::rotate(glm::mat4(1.0f), angle, right) * rot;
			dir = glm::mat3(rot) * glm::vec3(0.0f, 1.0f, 0.0f);
			right = glm::mat3(rot) * glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case '^':
			rot = glm::rotate(glm::mat4(1.0f), -angle, right) * rot;
			dir = glm::mat3(rot) * glm::vec3(0.0f, 1.0f, 0.0f);
			right = glm::mat3(rot) * glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case '/':
			rot = glm::rotate(glm::mat4(1.0f), angle, -dir) * rot;
			dir = glm::mat3(rot) * glm::vec3(0.0f, 1.0f, 0.0f);
			right = glm::mat3(rot) * glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case '\\':
			rot = glm::rotate(glm::mat4(1.0f), angle, dir) * rot;
			dir = glm::mat3(rot) * glm::vec3(0.0f, 1.0f, 0.0f);
			right = glm::mat3(rot) * glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case '|':
			rot = glm::rotate(glm::mat4(1.0f), glm::pi<float>(), right) * rot;
			dir = -dir;
			right = -right;
			break;
		}
	}
}
