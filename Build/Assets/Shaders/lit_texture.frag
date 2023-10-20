#version 430

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
	vec3 position;
	vec3 ambientColor;
	vec3 diffuseColor;
} light;

layout(binding = 0) uniform sampler2D tex;


void main()
{
	// ambient
	vec3 ambient = light.ambientColor;

	// diffuse
	vec3 lightDir = normalize(light.position - fposition);
	float intensity = max(dot(lightDir, fnormal), 0);
	vec3 diffuse = material.diffuse * (light.diffuseColor * intensity);

	// specular
	vec3 viewDir = normalize(-fposition);
	vec3 reflectDir = reflect(-lightDir, fnormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.specular * light.diffuseColor * spec;
	
	// texture and lighting
	vec4 texcolor = texture(tex, ftexcoord);

	vec3 lighting = ambient + diffuse + specular;
	vec3 color = lighting * texcolor.rgb;

	ocolor = vec4(color, 1.0);
}