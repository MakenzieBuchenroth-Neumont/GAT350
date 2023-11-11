#version 430

#define INVERT_MASK    (1 << 0)
#define GRAYSCALE_MASK (1 << 1)
#define COLORTINT_MASK (1 << 2)
#define GRAIN_MASK     (1 << 3)
#define SCANLINE_MASK  (1 << 4)
#define BLOOM_MASK     (1 << 5)

in layout(location = 0) vec2 ftexcoord;
out layout(location = 0) vec4 ocolor;

uniform float blend = 1;
uniform uint params = 0;
uniform vec3 tintcolor = { 0,0,0 };
uniform float time = 0;
uniform float bloomThreshold;
uniform float bloomIntensity;


layout(binding = 0) uniform sampler2D screenTexture;

float random(vec2 st) {
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec4 invert(in vec4 color) {
	return vec4(vec3(1) - color.rgb, color.a);
}

vec4 grayscale(in vec4 color) {
	return vec4(vec3(0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b), color.a);
}

vec4 tint(in vec4 color, in vec3 tintcolor) {
	return vec4((color.r + tintcolor.r) / 2, (color.g + tintcolor.g) /2, (color.b + tintcolor.b) / 2, (color.a));
}

vec4 grain(in vec4 color) {

	return color * random(gl_FragCoord.xy + time);
}

vec4 scanline(in vec4 color) {
	return (mod(int(gl_FragCoord.y), 2) != 0 ) ? vec4(0) : color;
}

vec3 guassianBlur(in sampler2D textureSampler, in vec2 ftexcoord, float offset, vec2 textureSize) {
	vec3 result = vec3(0.0);
	float totalWeight = 0;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			vec2 offsetCoords = ftexcoord + vec2(i, j) * offset;
			vec3 sampled = texture(textureSampler, offsetCoords).rgb;

			float weight = exp(-(i * i * j * j) / (2.0 * offset * offset));

			result += sampled * weight;
			totalWeight += weight;

		}
	}

	return result / totalWeight;
}

vec4 bloom(in vec4 color) {
	float luminance = dot(color.rgb, vec3(0.299, 0.587, 0.114));

	if (luminance > bloomThreshold) {
	vec3 blurredColor = guassianBlur(screenTexture, ftexcoord, 0.1, vec2(800.0, 600.0));
	color.rgb += blurredColor * bloomIntensity;
	}
	return color;
}


void main()
{
	vec4 basecolor = texture(screenTexture, ftexcoord);
	vec4 postprocess = basecolor;

	if (bool(params & INVERT_MASK))	   postprocess = invert(postprocess); 
	if (bool(params & GRAYSCALE_MASK)) postprocess = grayscale(postprocess);
	if (bool(params & COLORTINT_MASK)) postprocess = tint(postprocess, tintcolor);
	if (bool(params & GRAIN_MASK))     postprocess = grain(postprocess);
	if (bool(params & SCANLINE_MASK))  postprocess = scanline(postprocess);
	if (bool(params & BLOOM_MASK))     postprocess = bloom(postprocess);


	ocolor = mix(basecolor, postprocess, blend);
}