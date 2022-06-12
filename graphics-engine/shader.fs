#version 460 core
	
in vec3 fragPosition;
in vec3 normal;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;

void main()
{
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  vec3 norm = normalize(normal);
  vec3 lightDirection = normalize(lightPosition - fragPosition);
  float diff = max(dot(norm, lightDirection), 0.0);
  vec3 diffuse = diff * lightColor;

  fragColor = vec4((ambient + diffuse) * objectColor, 1.0);
}