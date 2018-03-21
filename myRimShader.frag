#version 330
layout(location = 0) out vec4 outColor;

in vec3 worldPos;
in vec3 worldNormal;
in vec2 texCoords;

uniform vec3 lightPosition;
uniform vec3 eyePosition;

uniform sampler2D texture_diffuse1;

//adjust as desired original test = 0.15, 0.05, 0.0,    rim = 0.2,0.2,0.2, blue = 0.101, 0.368, 0.858, gamma orig = 1.0/0.6
const vec3 diffuseLight = vec3(0.15, 0.05, 0.0);
const vec3 rimColor  = vec3(0.101, 0.368, 0.858);

//higher gamma equals darker image
const float gamma = 1.0/0.4;



void main(){

	vec3 tex = texture(texture_diffuse1, texCoords).rgb;

	//get light and view directions
	vec3 L = normalize( lightPosition - worldPos);
	vec3 V = normalize( eyePosition - worldPos);

	//diffuse lighting
	vec3 diffuse = diffuseLight * max(0, dot(L,worldNormal));
	
	//rim lighting
	float rim = 1 - max(dot(V, worldNormal), 0.0);
	rim = smoothstep(0.6, 1.0, rim);
	vec3 finalRim = rimColor * vec3(rim, rim, rim);

	//Lights + Texture
	vec3 finalColor = finalRim + diffuse + tex;
	vec3 finalColorGamma = vec3(pow(finalColor.r, gamma),
								pow(finalColor.g, gamma),
								pow(finalColor.b, gamma));

	outColor = vec4(finalColorGamma, 1);
}