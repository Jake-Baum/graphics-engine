#version 460 core
	
in vec3 color;
in vec2 textureCoord;

out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
  fragColor = vec4(lightColor * objectColor, 1.0);
}