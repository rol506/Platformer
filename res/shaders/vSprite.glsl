#version 330 core

layout(location=0) in vec2 vertexPosition;
layout(location=1) in vec2 aTexCoord;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

uniform float layer;

out vec2 texCoord;
out vec2 vertexPos;

void main()
{
	texCoord = aTexCoord;
	
	gl_Position = projectionMatrix * modelMatrix * vec4(vertexPosition, layer, 1.0);
	vertexPos = vertexPosition;
}