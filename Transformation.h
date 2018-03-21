#pragma once
#include "Node.h"
class Transformation :
	public Node
{
public:
	void draw(glm::mat4 M);
	void update();
	void regenerate();

	void addChild(Node* child);
	void removeChild(Node* child);
	Transformation(glm::mat4 T);
	~Transformation();

protected:
	glm::mat4 T;
	std::vector<Node*> children;

private:
};

