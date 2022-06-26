#version 460 core

struct Material {
  sampler2D textureDiffuse1;
  sampler2D textureDiffuse2;
  sampler2D textureDiffuse3;
  sampler2D textureSpecular1;
  sampler2D textureSpecular2;
  float shininess;
};

struct DirectionalLight {
  vec3 direction;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {    
  vec3 position;
    
  float constant;
  float linear;
  float quadratic;  

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};  

#define NR_POINT_LIGHTS 1  

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection);  
	
in vec3 fragPosition;
in vec3 normal;
in vec2 textureCoords;

out vec4 fragColor;

uniform vec3 viewPosition;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

void main()
{
  vec3 norm = normalize(normal);
  vec3 viewDirection = normalize(viewPosition - fragPosition);

  vec3 result = calculateDirectionalLight(directionalLight, norm, viewDirection);

  for (int i = 0; i < NR_POINT_LIGHTS; i++) 
  {
    result += calculatePointLight(pointLights[i], norm, fragPosition, viewDirection);
  }

  fragColor = vec4(result, 1.0);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
  vec3 lightDirection = normalize(-light.direction);
  float diff = max(dot(normal, lightDirection), 0.0);
  
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
  
  vec3 ambient = light.ambient * vec3(texture(material.textureDiffuse1, textureCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.textureDiffuse1, textureCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.textureSpecular1, textureCoords));
  return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
  vec3 lightDirection = normalize(light.position - fragPosition);
  float diff = max(dot(normal, lightDirection), 0.0);

  vec3 reflectDirection = reflect(-lightDirection, normal);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

  float distance = length(light.position - fragPosition);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

  vec3 ambient = light.ambient * vec3(texture(material.textureDiffuse1, textureCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.textureDiffuse1, textureCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.textureSpecular1, textureCoords));
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular);
}