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

				layout (location = 0) out vec4 fragColor;

				in vec2 v_uv;

				vec4 getText() {
					vec3 textColor = vec3(0f, 0f, 0f);
					vec3 white = vec3(1f, 1f, 1f);

					float d = texture2D(source, v_uv).x - 0.48;	

					if (d > 0.0) {
						return vec4(textColor, 1f);
					} 
					else {
						return vec4(textColor, 0f);
					}
				}

				vec4 getWithOutlineText() {
					vec3 outlineColor = vec3(0.2, 0.7, 0.898);
					vec3 textColor = vec3(1.0, 0.733, 0.2);
					vec4 color = texture2D(source, v_uv);

					float d = color.x - 0.48;	

					float a = smoothstep(-0.02, 0.00 , d);

					// Interpolations Faktor zwischen outline und Welt
					float ao = smoothstep(-0.2, -0.00 , d);

					if (d > 0.0) {
						return vec4(textColor, 1.0);
					}
					else if (a > 0.0) {
						return vec4(outlineColor * (1.0 - a) + textColor * a, 1.0);
					}
					else {
						return vec4(outlineColor, ao);
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

					vec4 diffuse = getWithOutlineText() * angle;
					return diffuse;
				}

				void main()
				{
					fragColor = getText();
				}
				)";
	}; // Shader
}; // glat