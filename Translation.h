#pragma once
#include "Transformation.h"

class Translation :
	public Transformation
{
public:
	Translation();
	~Translation();

	void moveBy(glm::vec3 dx);
	void rotateBy(glm::mat4 R);

};

