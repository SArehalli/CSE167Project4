#include "Transformation.h"



void Transformation::draw(glm::mat4 M)
{
	for (int i = 0; i < children.size(); i++) {
		children.at(i)->draw(M * T);
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
