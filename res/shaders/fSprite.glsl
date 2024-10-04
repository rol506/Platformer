#version 330 core

in vec2 texCoord;

uniform sampler2D tex;
uniform int outline;

out vec4 OutColor;

void main()
{
	if (outline == 1)
	{
		OutColor = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
		OutColor = texture(tex, texCoord);
	}
	
	if (OutColor.rgb == vec3(0.0) && outline != 1)
	{
		discard;
	}
}