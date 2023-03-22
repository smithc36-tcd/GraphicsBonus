#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

out vec3 finalColour;

uniform mat4 camMatrix;
uniform vec3 cameraPosition;

const vec3 lightDirection = normalize(vec3(0.4, -1.0, 0.8));
const vec3 waterColour = vec3(0.2, 0.4, 0.45);
const vec3 lightColour = vec3(1.0, 0.6, 0.6);
const float reflectivity = 0.5;
const float shineDamper = 8.0;
const float ambientLighting = 0.3;

vec3 calcNormals(){
    vec3 t1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    vec3 t2 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
    /*vec3 normal = cross(t1, t2);*/
    return normalize(cross(t1, t2));
}

vec3 specularLight(vec4 worldPosition, vec3 normal){
    vec3 viewVector = normalize(cameraPosition - worldPosition.xyz);
	vec3 reflectedLightDirection = reflect(lightDirection, normal);
	float specularFactor = dot(reflectedLightDirection, viewVector);
	specularFactor = max(pow(specularFactor, shineDamper), 0.0);
	return lightColour * specularFactor * reflectivity;
}


void main()
{
	vec3 normal = calcNormals();
	float brightness = max(dot(-lightDirection, normal), ambientLighting);
	/*vec3 colour = waterColour * brightness;*/

	vec4 worldPosition = gl_in[0].gl_Position;
	gl_Position = camMatrix * worldPosition;
	finalColour = gl_in[0].gl_Position.y + specularLight(worldPosition, normal);
	EmitVertex();
	
	worldPosition = gl_in[1].gl_Position;
	gl_Position = camMatrix * worldPosition;
	finalColour = gl_in[1].gl_Position.y + specularLight(worldPosition, normal);
	EmitVertex();
	
	worldPosition = gl_in[2].gl_Position;
	gl_Position = camMatrix * worldPosition;
	finalColour = gl_in[2].gl_Position.y+ specularLight(worldPosition, normal);
	EmitVertex();
	
	EndPrimitive();
}

