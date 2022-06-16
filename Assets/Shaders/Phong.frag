#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 WorldPos;

out vec4 FragColor;

uniform sampler2D DiffuseTexture;
uniform float AmbientIntensity;
uniform vec3 LightCol;
uniform vec3 LightDir;
uniform vec3 WorldCameraPos;

void main() 
{
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(LightDir);
	vec3 ambientCol = LightCol * AmbientIntensity;

	float diffuseIntensity = dot(normal, -lightDir);
	diffuseIntensity = max(diffuseIntensity, 0.0);
	vec3 diffuseCol = LightCol * diffuseIntensity;

	vec3 viewDir = normalize(WorldCameraPos - WorldPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	FragColor = texture(DiffuseTexture, TexCoords);
	FragColor *= vec4(ambientCol + diffuseCol, 1.0);
}