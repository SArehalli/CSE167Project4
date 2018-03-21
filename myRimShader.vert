#version 330
 
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 aTexCoords;
 
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 worldPos;
out vec3 worldNormal;
out vec2 texCoords;
 
void main()
{
 //convert world coords
 worldPos = mat3(model) * position;//grab upper left from mat4 model
 worldNormal = normalize(mat3(model) * normal); 
 texCoords = aTexCoords;
 
 gl_Position = projection * view * model * vec4(position, 1.0);
}