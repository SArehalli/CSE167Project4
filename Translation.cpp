#include "Translation.h"

Translation::Translation()
	: Transformation(glm::mat4(1.0f))
{
}

Translation::~Translation()
{
}

void Translation::moveBy(glm::vec3 dx)
{
	this->T = glm::translate(glm::mat4(1.0f), dx) * this->T;
}

void Translation::rotateBy(glm::mat4 R)
{
	this->T = this->T * R;
}
