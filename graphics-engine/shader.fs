#version 460 core
	
in vec3 color;
in vec2 textureCoord;

out vec4 fragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
  fragColor = mix(texture(texture1, textureCoord), texture(texture2, textureCoord), 0.5);
}