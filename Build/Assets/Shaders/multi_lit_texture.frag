#version 430

#define POINT		0
#define DIRECTIONAL 1
#define SPOT		2

//in layout(location = 0) vec3 color;
in layout(location = 0) vec3 fposition;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec2 ftexcoord;

out layout(location = 0) vec4 ocolor;

uniform struct Material {
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec2 tiling;
	vec2 offset;
} material;

uniform struct Light {
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float range;
	float innerAngle;
	float outerAngle;
} lights[3];

uniform vec3 ambientLight;
uniform int numLights = 3;

layout(binding = 0) uniform sampler2D tex;

void phong(in Light light, in vec3 position, in vec3 normal, out vec3 diffuse, out vec3 specular) {
	// diffuse
	vec3 lightDir = (light.type == DIRECTIONAL) ? normalize(-light.direction) : normalize(light.position - fposition);

	float spotIntensity = 1;
	if (light.type == SPOT) {
		float angle = acos(dot(light.direction, -lightDir));
		spotIntensity = smoothstep(light.outerAngle + 0.001, light.innerAngle, angle);
	}

	float intensity = max(dot(lightDir, fnormal), 0) * spotIntensity;
	diffuse = material.diffuse * (light.color * intensity);

	// specular
	specular = vec3(0);
	if (intensity > 0) {
		vec3 reflectDir = reflect(-lightDir, fnormal);
		vec3 viewDir = normalize(-fposition);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specular = material.specular * intensity * spotIntensity;
	}
}

float attenuation(in vec3 position1, in vec3 position2, in float range)
{
	float distanceSqr = dot(position1 - position2, position1 - position2);
	float rangeSqr = pow(range, 2.0);
	float attenuation = max(0, 1 - pow((distanceSqr / rangeSqr), 2.0));
	attenuation = pow(attenuation, 2.0);
 
	return attenuation;
}

void main()
{
	// texture and lighting
	vec4 texcolor = texture(tex, ftexcoord);
	// set ambient light
	ocolor = vec4(ambientLight, 1) * texcolor;

	for (int i = 0; i < numLights; i++) {
		vec3 diffuse;
		vec3 specular;
 
		float attenuation = (lights[i].type == DIRECTIONAL) ? 1 : attenuation(lights[i].position, fposition, lights[i].range);
 
		phong(lights[i], fposition, fnormal, diffuse, specular);
		ocolor += ((vec4(diffuse, 1) * texcolor) + vec4(specular, 1)) * attenuation * lights[i].intensity;
	}
}