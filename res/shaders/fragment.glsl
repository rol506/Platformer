#version 330 core

in vec3 Color;
in vec2 TexCoord;

out vec4 OutColor;

uniform float timer;
uniform sampler2D tex;

void main()
{
	//OutColor = vec4(Color.xy, timer, 1.0);
	OutColor = texture(tex, TexCoord+vec2(timer, 0));
}