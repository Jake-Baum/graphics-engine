#version 460 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
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
  vec3 lightDirection = normalize(light.position - fragPosition);
  float theta = dot(lightDirection, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

  float distance = length(light.position - fragPosition);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

  vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureCoords)) * attenuation;

  vec3 norm = normalize(normal);
  float diff = max(dot(norm, lightDirection), 0.0);
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, textureCoords)) * attenuation * intensity;

  vec3 viewDirection = normalize(viewPosition - fragPosition);
  vec3 reflectDirection = reflect(-lightDirection, norm);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
  vec3 specular = light.specular * spec * vec3(texture(material.specular, textureCoords)) * attenuation * intensity;
  

  fragColor = vec4(ambient + diffuse + specular, 1.0);
}