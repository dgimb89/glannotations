#version 140
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D color;
uniform sampler2D normal;
uniform sampler2D geom;

layout (location = 0) out vec4 fragColor;

in vec2 v_uv;
in vec3 v_eye;

const vec3 lpos = vec3(0.0, 2.0, 2.0);

const vec3  specular = vec3(1.0, 1.0, 1.0) * 0.1;
const float shininess = 128.0;
const vec3  ambient = vec3(0.08, 0.10, 0.14);
const float ambifake = 0.78;

void main()
{
	vec3 n = normalize(texture(normal, v_uv).rgb);
	vec3 g = texture(geom, v_uv).rgb;
	
	vec3 l = normalize(lpos - g);
	vec3 e = normalize(vec3(1.f, 1.f, 1.f));
	vec3 h = normalize(l + e);

	float ldotn = mix(ambifake, 1.0, dot(l, n));
	float hdotn = dot(h, n);

	vec4 phong;

	if(ldotn > 0.0)
		phong = vec4(
		  specular * clamp(pow(hdotn, shininess), 0.0, 1.0) 
		+ mix(ambient, vec3(1.0), ldotn), 1.0);
	else
		phong = vec4(ambient, 1.0);

	fragColor = (texture(color, v_uv)) * phong;
	//fragColor = phong;
}