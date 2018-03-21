#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoords;

//out vec3 Normal;
//out vec3 FragPos;
out vec2 TexCoords;
//out float sampleExtraOutput;

//uniform mat4 modelview;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	mat3 scale = mat3(model);
	int x,y;
	if (abs(normal.x) >= 0.5) {
		x = 1;
		y = 2;
	} else if (abs(normal.z) >= 0.5) {
		x = 1;
		y = 0;
	} else {
		x = 0;
		y = 2;
	}
    TexCoords = aTexCoords;
	TexCoords.x *= scale[x][x];
	TexCoords.y *= scale[y][y];
    gl_Position = projection * view * model * vec4(position, 1.0);

    //gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
    //sampleExtraOutput = 1.0f;

    //gl_Position = projection * view *  model * vec4(position, 1.0f);
    //FragPos = vec3(model * vec4(position, 1.0f));
    //Normal = mat3(transpose(inverse(model))) * normal;  
    //TexCoords = texCoords;
} 