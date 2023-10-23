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
	float cutoff;
} light;

uniform vec3 ambientLight;

layout(binding = 0) uniform sampler2D tex;

vec3 ads(in vec3 position, in vec3 normal) {
	//ambient
	vec3 ambient = ambientLight;

	// diffuse
	vec3 lightDir = (light.type == DIRECTIONAL) ? normalize(-light.direction) : normalize(light.position - fposition);

	float spotIntensity = 1;
	if (light.type == SPOT) {
		float angle = acos(dot(light.direction, -lightDir));
		if (angle > light.cutoff) spotIntensity = 0;
	}

	float intensity = max(dot(lightDir, fnormal), 0);
	vec3 diffuse = material.diffuse * (light.color * intensity * spotIntensity);



	// specular
	vec3 specular = vec3(0);
	if (intensity > 0) {

	vec3 viewDir = normalize(-fposition);
	vec3 reflectDir = reflect(-lightDir, fnormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * intensity;
}

return ambient + diffuse + specular;

}

void main()
{
	// texture and lighting
	vec4 texcolor = texture(tex, ftexcoord);

	ocolor = texcolor * vec4(ads(fposition, fnormal), 1);
}