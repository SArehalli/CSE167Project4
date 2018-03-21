#include "Maze.h"



Maze::Maze(int n, GLint shader) :
	LSystem(n)
{
	this->n = n;
	this->shader = shader;
	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		for (int j = 0; j < MAZE_WIDTH; j++)
		{
			maze[i][j] = 0;
		}
	}

	addRule('X', "[FG]-[FG]-[FG]-[FG]");
	addRule('G', "FG");
	addRule('G', "-FG");
	addRule('G', "+FG");
	addRule('G', "-[FG]+[FG]");
	build();
}


Maze::~Maze()
{
}

void Maze::regenerate()
{
	this->rep = "X";
	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		for (int j = 0; j < MAZE_WIDTH; j++)
		{
			maze[i][j] = 0;
		}
	}
	for (int i = 0; i < children.size(); i++) delete children.at(i);
	children.clear();
	build();
}

struct Pos {
	int x, y;
};

void validate(struct Pos *pos)
{
	if (pos->x < 1) pos->x = 1;
	if (pos->y < 1) pos->y = 1;
	if (pos->x >= MAZE_WIDTH - 1) pos->x = MAZE_WIDTH - 2;
	if (pos->y >= MAZE_WIDTH - 1) pos->y = MAZE_WIDTH - 2;
}

void Maze::build()
{	
	for (int i = 0; i < n; i++) iterate();

	Pos pos;
	pos.x = pos.y = MAZE_WIDTH/2;
	int dir = 0;

	Model *wall = new Model("../objs/cube.obj", shader);

	std::stack<struct Pos> posStack;
	std::stack<int> dirStack;

	// Parse string to 2x2 map "maze"
	for (int idx = 0; idx < rep.size(); idx++)
	{
		char c = rep[idx];
		int length;
		switch (c)
		{
		case 'F':
			length = rand() % 3 + 1;
			for (int i = 0; i < length; i++)
			{
				switch (dir)
				{
				case 0:
					maze[pos.x][pos.y] = 1;
					pos.y++;
					break;
				case 1:
					maze[pos.x][pos.y] = 1;
					pos.x++;
					break;
				case 2:
					maze[pos.x][pos.y] = 1;
					pos.y--;
					break;
				case 3:
					maze[pos.x][pos.y] = 1;
					pos.x--;
					break;
				}
				validate(&pos);
			}
			break;
		case '-':
			dir = (dir + 3) % 4;
			break;
		case '+':
			dir = (dir + 1) % 4;
		case '[':
			dirStack.push(dir);
			posStack.push(pos);
			break;
		case ']':
			dir = dirStack.top();
			pos = posStack.top();
			dirStack.pop();
			posStack.pop();
		}
	}
	// DEBUG 
	for (int i = MAZE_WIDTH - 1; i >= 0; i--)
	{
		for (int j = 0; j < MAZE_WIDTH; j++)
		{
			printf("%d ", maze[j][i]);
		}
		printf("\n");
	}

	// Convert grid to wall blocks
	for (int i = 0; i < MAZE_WIDTH; i++)
	{
		for (int j = 0; j < MAZE_WIDTH; j++)
		{
			if (maze[i][j] == 0)
			{
				// Find the largest square you can
				int i_ = i;
				while (maze[i_][j] == 0 && i_ < MAZE_WIDTH) i_++;
				int j_ = j;
				while (maze[i][j_] == 0  && j_ < MAZE_WIDTH)
				{
					bool new_col = true;
					for (int k = i; k < i_; k++)
					{
						if (maze[k][j_] != 0) new_col = false;
					}
					if (!new_col) break;
					j_++;
				}

				// Add to scene graph
				printf("%d x %d at (%d, %d)\n", i_ - i, j_ - j, i, j);
				glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2 * (i_ - i), 2, 2 * (j - j_)));
				glm::mat4 move = glm::translate(glm::mat4(1.0f), glm::vec3(2 * i - MAZE_WIDTH, 0, 2 * j - MAZE_WIDTH));
				Transformation *transform = new Transformation(move * scale);
				this->addChild(transform);
				transform->addChild(wall);

				// Block off for future passes
				for (int k = i; k < i_; k++)
				{
					for (int l = j; l < j_; l++)
					{
						maze[k][l] = 1;
					}
				}
				j = j_ - 1;

			}
		}
	}
}
