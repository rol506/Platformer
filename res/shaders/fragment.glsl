#version 330 core

in vec3 Color;

out vec4 OutColor;

uniform float timer;

void main()
{
	OutColor = vec4(Color.xy, timer, 1.0);
}