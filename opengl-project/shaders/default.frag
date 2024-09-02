#version 410 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D diffTexture;
uniform sampler2D specTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

vec4 pointLight()
{
    vec3 lightVec = lightPos - FragPos;
    float distance = length(lightVec);

    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;

    // Calculate attenuation
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightVec);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Apply attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // Combine results
    return texture(diffTexture, TexCoord) * vec4(ambient + diffuse, 1.0) + vec4(specular, 1.0) * texture(specTexture, TexCoord).r;
}

vec4 directionalLight()
{
    // Ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(1.0, 1.0, 0.0)); // Direction of the light
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * lightColor;

    return vec4(ambient + diffuse + specular, 1.0) * texture(diffTexture, TexCoord) * texture(specTexture, TexCoord).r;
}  

vec4 spotLight()
{   
    vec3 lightVec = lightPos - FragPos;
    vec3 lightDir = normalize(lightVec);

    float outerCone = 0.90f;
    float innerCone = 0.95f;

    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor * texture(diffTexture, TexCoord).rgb;

    // Diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * texture(diffTexture, TexCoord).rgb;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor * texture(specTexture, TexCoord).rgb;

    // Calculate spotlight intensity based on the cone angles
    float angle = dot(lightDir, normalize(-lightPos)); // Should use spotlight direction
    float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0, 1.0);

    return vec4((ambient + diffuse + specular) * intensity, 1.0) * texture(diffTexture, TexCoord) * texture(specTexture, TexCoord).r;
}


void main()
{
    FragColor = pointLight();
}
