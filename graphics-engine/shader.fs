#version 460 core

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
	
in vec3 fragPosition;
in vec3 normal;

out vec4 fragColor;

uniform vec3 viewPosition;
uniform Material material;
uniform Light light;

void main()
{
  vec3 ambient = light.ambient * material.ambient;

  vec3 norm = normalize(normal);
  vec3 lightDirection = normalize(light.position - fragPosition);
  float diff = max(dot(norm, lightDirection), 0.0);
  vec3 diffuse = light.diffuse * (diff * material.diffuse);

  vec3 viewDirection = normalize(viewPosition - fragPosition);
  vec3 reflectDirection = reflect(-lightDirection, norm);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
  vec3 specular = light.specular * (spec * material.specular);

  fragColor = vec4(ambient + diffuse + specular, 1.0);
}