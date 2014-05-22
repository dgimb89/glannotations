#version 140
#extension GL_ARB_explicit_attrib_location : require

layout (location = 0) out vec4 colorOut;
layout (location = 1) out vec4 normalOut;

smooth in vec3 v_normal;

void main()
{
	vec3 normal = normalize(v_normal);
	normalOut = vec4(0.f, 1.f, 0.f, 1.f);//vec4(normal, 1.0);
	colorOut = vec4(0.f, 1.f, 0.f, 1.f);// gl_FragColor;
}
