#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;


out vec3 Normal;
out vec3 Position;

uniform mat4 mvp;
uniform mat4 model;


void main()
{
	Position = vec3(model * vec4(aPos, 1.0)); 
	
	Normal = vec3(model * vec4(aNormal, 0.0));
    gl_Position = mvp * vec4(Position, 1.0);
}  