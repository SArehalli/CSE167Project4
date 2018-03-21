#include "Tree.h"



Tree::Tree(int n, Shader *shader) :
	LSystem(n)
{
	this->n = n;
	this->shader = shader;
	addRule('X', "F[-FX][+FX]");
	addRule('X', "F[^FX][&FX]");
	addRule('X', "F[^FX]//////[^FX]/////[^FX]");
	build(n);
}

void Tree::regenerate()
{
	this->rep = "X";
	for (int i = 0; i < children.size(); i++) delete children.at(i);
	children.clear();

	build(n);
}

Tree::~Tree()
{
}

void Tree::build(int n)
{
	for (int i = 0; i < n; i++) iterate();

	GLfloat angle = 45.0f/360.0f * glm::pi<float>();
	glm::vec3 dir = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

	Model *branch = new Model("../objs/cylinder2.obj", shader);

	std::stack<glm::mat4> rotStack;
	std::stack<glm::mat4> movStack;
	std::stack<glm::mat4> sizeStack;

	Transformation *rotT, *movT, *sizeT;
	glm::mat4 rot, mov, size;

	for (int idx = 0; idx < rep.size(); idx++) {
		char c = rep[idx];
		movT = new Transformation(mov);
		switch (c) 
		{
		case 'F':
			movT = new Transformation(mov);
			rotT = new Transformation(rot);
			sizeT = new Transformation(size);
			this->addChild(movT);
			movT->addChild(rotT);
			rotT->addChild(sizeT);
			sizeT->addChild(branch);
		case 'f':
			mov = glm::translate(glm::mat4(1.0f), glm::mat3(size) * dir) * mov;
			size *= glm::scale(glm::mat4(1.0f), glm::vec3(0.8f));
			break;

		case '[':
			rotStack.push(rot);
			movStack.push(mov);
			sizeStack.push(size);
			break;
		case ']':
			mov = movStack.top();
			rot = rotStack.top();
			size = sizeStack.top();
			movStack.pop();
			rotStack.pop();
			sizeStack.pop();
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
