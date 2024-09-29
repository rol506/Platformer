#version 330 core

in vec2 texCoord;

uniform sampler2D tex;

out vec4 OutColor;

void main()
{
	OutColor = texture(tex, texCoord);
	//OutColor = vec4(0.0, 0.0, 0.0, 1.0);
	
	if (OutColor.rgb == vec3(0.0))
	{
		discard;
	}
}