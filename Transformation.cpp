#include "Transformation.h"



void Transformation::draw(glm::mat4 M)
{
	for (int i = 0; i < children.size(); i++) {
		children.at(i)->draw(M * T);
	}
}
void Transformation::Draw(glm::mat4 M, GLint shader) {
   for (int i = 0; i < children.size(); i++) {
      children.at(i)->draw(M * T);
   }
}

void Transformation::DrawWBB(GLint shaderOBJ, GLint shaderBB, glm::mat4 obj2World) {
   for (int i = 0; i < children.size(); i++) {
      children.at(i)->DrawWBB(shaderOBJ, shaderBB, obj2World * T);
   }
}

void Transformation::update()
{
	for (int i = 0; i < children.size(); i++) {
		children.at(i)->update();
	}
}

void Transformation::regenerate()
{
	for (int i = 0; i < children.size(); i++) {
		children.at(i)->regenerate();
	}
}

void Transformation::addChild(Node * child)
{
	children.push_back(child);
}

void Transformation::removeChild(Node * child)
{
	// TODO
}

Transformation::Transformation(glm::mat4 T)
{
	this->T = T;
}


Transformation::~Transformation()
{
	for (int i = 0; i < children.size(); i++) {
		delete children.at(i);
	}
}
