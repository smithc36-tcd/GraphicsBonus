#version 330 core
out vec4 FragColor;

in vec3 currPos;
in vec3 Normal; 
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;


vec4 PointLight()
{
    vec3 lightVec = lightPos - currPos;
    float dist = length(lightVec);
    float a = 1.0, b = 0.7;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);
    
    // To simulate light bouncing
    float ambient = 0.2f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);

    float diffuse= max(dot(normal, lightDirection), 0.0f);
    
    float specularLight = 0.5f;
    vec3 viewDirection = normalize(camPos - currPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    float specular = specAmount * specularLight;

    return (texture(diffuse0, TexCoord) * lightColor * (diffuse * inten + ambient) + 
        texture(specular0, TexCoord).r * specular * inten) * lightColor;
}

vec4 DirectionalLight()
{
    vec3 lightVec = lightPos - currPos;
    
    // To simulate light bouncing
    float ambient = 0.2f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));

    float diffuse= max(dot(normal, lightDirection), 0.0f);
    
    float specularLight = 0.5f;
    vec3 viewDirection = normalize(camPos - currPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    float specular = specAmount * specularLight;

    return (texture(diffuse0, TexCoord) * lightColor * (diffuse + ambient) + 
        texture(specular0, TexCoord).r * specular) * lightColor;
}

vec4 SpotLight()
{
    float outerCone = 0.9f;
    float innerCone = 0.95f;

    vec3 lightVec = lightPos - currPos;
    
    // To simulate light bouncing
    float ambient = 0.2f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);

    float diffuse= max(dot(normal, lightDirection), 0.0f);
    
    float specularLight = 0.5f;
    vec3 viewDirection = normalize(camPos - currPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    float specular = specAmount * specularLight;


    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    return (texture(diffuse0, TexCoord) * lightColor * (diffuse * inten + ambient) + 
        texture(specular0, TexCoord).r * specular * inten) * lightColor;
}

void main()
{
    //FragColor = PointLight();
    FragColor = DirectionalLight();
    //FragColor = SpotLight();
}
