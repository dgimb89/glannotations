namespace glat {
	namespace ShaderSource {
		static const char*	vertexShaderSource = R"(
				#version 330

				layout (location = 0) in vec4 position;
				uniform vec2 scale;
				uniform vec2 offset;
				out vec2 v_uv;

				void main()
				{
					v_uv = vec2(position.x * scale.x, position.y * scale.y) * 0.5 + 0.5;
					gl_Position = position + vec4(offset.x, offset.y, 0f, 0f);
				}
				)";

		static const char* fragmentShaderSource = R"(
				#version 330

				uniform sampler2D source;
				uniform vec3 outlineColor;
				uniform float outlineSize;
				uniform int style;

				layout (location = 0) out vec4 fragColor;

				in vec2 v_uv;

				vec4 getText() {
					vec3 textColor = vec3(0f, 0f, 0f);
					float d = texture2D(source, v_uv).x - 0.48;	

					if (d > 0.0) {
						return vec4(textColor, 1f);
					} 
					else {
						return vec4(textColor, 0f);
					}
				}

				vec4 getTextWithOutline() {
					vec3 textColor =  vec3(0f, 0f, 0f);
					float d = texture2D(source, v_uv).x - 0.48;	

					// Interpolations Faktor zwischen outline und Welt
					float d_outline = smoothstep(-outlineSize, -0.00 , d);

					if (d > 0.0) {
						return vec4(textColor, 1.0);
					}
					else if (d_outline > 0.0) {
						return vec4(outlineColor, 1.0);
					}
					else {
						return vec4(0f, 0f, 0f, 0f);
					}
				}

				vec4 get3DText() {
					float dx = dFdx(texture2D(source, v_uv).x) * 3.0; 
					float dy = dFdy(texture2D(source, v_uv).x) * 3.0;

					vec3 vx = vec3(1.0, 0.0, dx);
					vec3 vy = vec3(0.0, 1.0, dy);

					vec3 n = cross(vx, vy);
					vec3 lightSource = vec3(0.0, 0.5, 1.0);

					n = normalize(n);
					lightSource = normalize(lightSource);

					float angle = dot(n, lightSource);

					vec4 diffuse = getTextWithOutline() * angle;
					return diffuse;
				}

				void main()
				{
					if (style == 1) {
						fragColor = getTextWithOutline();
					} else {
						fragColor = getText();
					}
				}
				)";
	}; // Shader
}; // glat