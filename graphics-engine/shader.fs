#version 460 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
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
in vec2 textureCoords;

out vec4 fragColor;

uniform vec3 viewPosition;
uniform Material material;
uniform Light light;

void main()
{
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureCoords));

  vec3 norm = normalize(normal);
  vec3 lightDirection = normalize(light.position - fragPosition);
  float diff = max(dot(norm, lightDirection), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, textureCoords));

  vec3 viewDirection = normalize(viewPosition - fragPosition);
  vec3 reflectDirection = reflect(-lightDirection, norm);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, textureCoords));

  fragColor = vec4(ambient + diffuse + specular, 1.0);
}