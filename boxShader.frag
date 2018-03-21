#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;
uniform int boxFlag; // if 0, set color to 0,0,0,1
uniform int collisionFlag;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent. Green: 0.172, 0.729, 0.215 Red: 0.615, 0.027, 0.086
    color = vec4(0.172, 0.729, 0.215, 1.0);
}
