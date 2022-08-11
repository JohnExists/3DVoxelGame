#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in int aBlockFace;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

flat out int face;
out vec3 fragPos;
out vec2 texCoord;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	texCoord = aTexCoord;
	face = aBlockFace;
	fragPos = vec3(model * vec4(aPos, 1.0));		
}