#version 430

in layout(location = 0) vec3 vcolor;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec4 ocolor;

uniform vec4 color;
uniform vec2 offset;
uniform vec2 tiling;

layout(binding = 0) uniform sampler2D tex1;
layout(binding = 1) uniform sampler2D tex2;

void main()
{
	vec4 vtexcolor1 = texture(tex1, (vtexcoord * tiling) + offset);
	vec4 vtexcolor2 = texture(tex2, (vtexcoord * tiling) + offset);

	vec4 vtexcolor = mix(vtexcolor1, vtexcolor2, 0.6);

	ocolor = vtexcolor * color;
}