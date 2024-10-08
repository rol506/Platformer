#version 330 core

in vec2 texCoord;
in vec2 vertexPos;

uniform sampler2D tex;
uniform int outline;

out vec4 OutColor;

void main()
{
	float borderWidth = 0.0001;
	
	if ((vertexPos.x < borderWidth || vertexPos.x >= 0.01 - borderWidth ||
		vertexPos.y < borderWidth || vertexPos.y >= 0.01 - borderWidth) && outline == 1)
	{
		OutColor = vec4(0.0, 1.0, 0.0, 1.0);
	} else 
	{
		OutColor = texture(tex, texCoord);
	}
	
	if (OutColor.rgb == vec3(0.0))
	{
		discard;
	}
}