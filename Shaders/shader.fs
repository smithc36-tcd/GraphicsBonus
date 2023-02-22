#version 330 core
out vec4 FragColor;

in vec3 currPos;
in vec3 Normal; 
in vec4 Color;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
//uniform float height;


vec4 PointLight()
{
    vec3 lightVec = lightPos - currPos;
    float dist = length(lightVec);
    float a = 1.0, b = 0.7;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);
    
    // To simulate light bouncing
    float ambient = 0.1f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);

    float diffuse = max(dot(normal, lightDirection), 0.0f);
    
    float specularLight = 0.5f;
    vec3 viewDirection = normalize(camPos - currPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    float specular = specAmount * specularLight;

    return lightColor * (diffuse + ambient) + 
         specular * lightColor;
    //return lightColor * (diffuse * inten + ambient) + 
         //specular * inten * lightColor;
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

    return lightColor * (diffuse + ambient) + 
          specular * lightColor;
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

    return lightColor * (diffuse * inten + ambient) + 
         specular * inten * lightColor;
}

vec4 ColorHeight(float y)
{
    if(y < 0.1) // water
    { 
        return vec4(0.1, 0.1, 1.0, 1.0);
    }
    else if(y < 0.2) // sand
    { 
        return vec4(0.1, 0.7, 0.7, 1.0);
    }
    else if(y < 0.5) // grass
    { 
        return vec4(0.2, 1.0, 0.3, 1.0);
    }
    else
        return vec4(0.7, 0.7, 0.7, 1.0);
}

void main()
{
    FragColor = PointLight() + Color;
    //FragColor = DirectionalLight() + Color;
    //FragColor = SpotLight();

    //FragColor = Color;

}
