#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec2 otexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform struct Material {
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec2 tiling;
	vec2 offset;
} material;

void main() {
	// get into mvp format
	mat4 modelView = view * model;

	oposition = vec3(modelView * vec4(vposition, 1));
	onormal = normalize(mat3(modelView) * vnormal);
	otexcoord = (vtexcoord * material.tiling) + material.offset;

	// model - view - projection
	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(vposition, 1.0);
}
