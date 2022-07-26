#version 460 core

struct Material {
  sampler2D textureDiffuse;
  sampler2D textureSpecular;
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

#define NR_DIRECTIONAL_LIGHTS 8
#define NR_POINT_LIGHTS 8

vec4 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec4 calculatePointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection);  
	
in vec3 fragPosition;
in vec3 normal;
in vec2 textureCoords;

out vec4 fragColor;

uniform vec3 viewPosition;
uniform Material material;
uniform int numberOfDirectionalLights;
uniform DirectionalLight directionalLights[NR_DIRECTIONAL_LIGHTS];
uniform int numberOfPointLights;
uniform PointLight pointLights[NR_POINT_LIGHTS];

void main()
{
  vec3 norm = normalize(normal);
  vec3 viewDirection = normalize(viewPosition - fragPosition);

  vec4 result = vec4(0.0);
  for (int i = 0; i < numberOfDirectionalLights; i++) {
    result += calculateDirectionalLight(directionalLights[i], norm, viewDirection);
  }

  if (material.shininess > 0) {
  for (int i = 0; i < numberOfPointLights; i++) 
    {
      result += calculatePointLight(pointLights[i], norm, fragPosition, viewDirection);
    }
  }
  
  fragColor = result;
}

vec4 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
  vec3 lightDirection = normalize(-light.direction);
  float diff = max(dot(normal, lightDirection), 0.0);
  
  vec3 reflectDirection = reflect(-lightDirection, normal);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
  
  vec4 ambient = vec4(light.ambient, 1.0) * texture(material.textureDiffuse, textureCoords);
  vec4 diffuse = vec4(light.diffuse, 1.0) * diff * texture(material.textureDiffuse, textureCoords);
  vec4 specular = material.shininess != 0 ? vec4(light.specular, 1.0) * spec * texture(material.textureSpecular, textureCoords) : vec4(0);
  return ambient + diffuse + specular;
}

vec4 calculatePointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection)
{
  vec3 lightDirection = normalize(light.position - fragPosition);
  float diff = max(dot(normal, lightDirection), 0.0);

  vec3 reflectDirection = reflect(-lightDirection, normal);
  float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

  float distance = length(light.position - fragPosition);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

  vec4 ambient = vec4(light.ambient, 1.0) * texture(material.textureDiffuse, textureCoords);
  vec4 diffuse = vec4(light.diffuse, 1.0) * diff * texture(material.textureDiffuse, textureCoords);
  vec4 specular = material.shininess != 0 ? vec4(light.specular, 1.0) * spec * texture(material.textureSpecular, textureCoords): vec4(0);
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
  return ambient + diffuse + specular;
}