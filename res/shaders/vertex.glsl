#version 330 core

layout(location=0) in vec3 vertexPosition;
layout(location=1) in vec3 vertexColor;
layout(location=2) in vec2 aTexCoord;

out vec3 Color;
out vec2 TexCoord;

uniform mat4 modelMat;
uniform mat4 projectionMat;

void main()
{
	gl_Position = projectionMat * modelMat * vec4(vertexPosition, 1.0);
	Color = vertexColor;
	TexCoord = aTexCoord;
}