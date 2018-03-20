#include "Tree.h"



Tree::Tree(int n, Shader *shader) :
	LSystem(n)
{
	this->shader = shader;
	addRule('X', "-FX");
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
	Transformation *scenePtr = this;
	Model *branch = new Model("../objs/cylinder2.obj", shader);
	Transformation *branchResize = new Transformation(glm::mat4(1.0f));
	branchResize->addChild(branch);
	std::stack<Transformation*> ptrStack;
	std::stack<glm::vec3> dirStack;
	std::stack<glm::vec3> rightStack;

	Transformation *rotT, *move;
	glm::mat4 rot;

	for (int idx = 0; idx < rep.size(); idx++) {
		char c = rep[idx];
		Transformation *move = new Transformation(glm::translate(glm::mat4(1.0f), dir * branch_len));
		switch (c) 
		{
		case 'F':
			scenePtr->addChild(branchResize);
		case 'f':
			scenePtr->addChild(move);
			scenePtr = move;
			break;

		case '[':
			ptrStack.push(scenePtr);
			dirStack.push(dir);
			rightStack.push(right);
			break;
		case ']':
			scenePtr = ptrStack.top();
			right = rightStack.top();
			dir = dirStack.top();
			ptrStack.pop();
			rightStack.pop();
			dirStack.pop();
			break;

		case '+':
			rot = glm::rotate(glm::mat4(1.0f), angle, glm::cross(dir, right));
			rotT = new Transformation(rot);
			scenePtr->addChild(rotT);
			right = glm::mat3(rot) * right;
			dir = glm::mat3(rot) * dir;
			scenePtr = rotT;
			break;
		case '-':
			rot = glm::rotate(glm::mat4(1.0f), -angle, glm::cross(dir, right));
			rotT = new Transformation(rot);
			scenePtr->addChild(rotT);
			right = glm::mat3(rot) * right;
			dir = glm::mat3(rot) * dir;
			scenePtr = rotT;
			break;
		case '&':
			rot = glm::rotate(glm::mat4(1.0f), angle, right);
			rotT = new Transformation(rot);
			scenePtr->addChild(rotT);
			right = glm::mat3(rot) * right;
			dir = glm::mat3(rot) * dir;
			scenePtr = rotT;
			break;
		case '^':
			rot = glm::rotate(glm::mat4(1.0f), -angle, right);
			rotT = new Transformation(rot);
			scenePtr->addChild(rotT);
			right = glm::mat3(rot) * right;
			dir = glm::mat3(rot) * dir;
			scenePtr = rotT;
			break;
		case '/':
			rot = glm::rotate(glm::mat4(1.0f), angle, -dir);
			rotT = new Transformation(rot);
			scenePtr->addChild(rotT);
			right = glm::mat3(rot) * right;
			dir = glm::mat3(rot) * dir;
			scenePtr = rotT;
			break;
		case '\\':
			rot = glm::rotate(glm::mat4(1.0f), angle, dir);
			rotT = new Transformation(rot);
			scenePtr->addChild(rotT);
			right = glm::mat3(rot) * right;
			dir = glm::mat3(rot) * dir;
			scenePtr = rotT;
			break;
		case '|':
			rotT = new Transformation(glm::rotate(glm::mat4(1.0f), glm::pi<float>(), right));
			scenePtr->addChild(rotT);
			scenePtr = rotT;
			dir = -dir;
			right = -right;
			break;
		}
	}
}
