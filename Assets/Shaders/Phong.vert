#version 330 core

layout (location = 0) in vec3 attrPos;
layout (location = 1) in vec3 attrNormal;
layout (location = 2) in vec2 attrTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 WorldPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * View * Model * vec4(attrPos, 1.0);
	TexCoords = attrTexCoords;
	Normal = mat3(transpose(inverse(Model))) * attrNormal;
	WorldPos = vec3(Model * vec4(attrPos, 1.0));
}