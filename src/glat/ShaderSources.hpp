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
					gl_Position = position + vec4(offset.x, offset.y, 0.0, 0.0);
				}
				)";

		static const char* fragmentShaderSource = R"(
				#version 330

				uniform sampler2D source;
				uniform vec3 outlineColor;
				uniform float outlineSize;
				uniform int style;
				uniform float bumpIntensity;

				layout (location = 0) out vec4 fragColor;

				in vec2 v_uv;

				vec4 getText() {
					vec3 textColor = vec3(1.0, 0.733, 0.2);//vec3(0.0, 0.0, 0.0);
					float d = texture2D(source, v_uv).x - 0.48f;	

					if (d < 0.0) {
						return vec4(textColor, 1.0);
					} 
					else {
						return vec4(textColor, 0.0);
					}
				}

				vec4 getTextWithOutline() {
					vec3 textColor = vec3(1.0, 0.733, 0.2);
					float d = texture2D(source, v_uv).x - 0.48;	

					// Interpolations Faktor zwischen outline und Welt
					float d_outline = smoothstep(outlineSize, 0.0, d);

					if (d < 0.0) {
						return vec4(textColor, 1.0);
					}
					else if (d_outline > 0.0) {
						return vec4(outlineColor, 1.0);
					}
					else {
						return vec4(0.0, 0.0, 0.0, 0.0);
					}
				}

				float getBumpMapEffect() {
					float dx = dFdx(texture2D(source, v_uv).x) * 45.0 * bumpIntensity;
					float dy = dFdy(texture2D(source, v_uv).x) * 50.0 * bumpIntensity;

					vec3 vx = vec3(1.0, 0.0, dx);
					vec3 vy = vec3(0.0, 1.0, dy);

					vec3 n = cross(vx, vy);
					vec3 lightSource = vec3(1.5, 1.5, 3.0);

					n = normalize(n);
					lightSource = normalize(lightSource);

					//vec4 diffuse = vec4(getTextWithOutline().rgb * angle, getTextWithOutline().a);
					//vec4 diffuse = getTextWithOutline() * angle;
					return dot(n, lightSource);
				}

				void main()
				{
					if (style == 1) {
						fragColor = getTextWithOutline();
					} else if (style == 2) {
						vec4 textColor = getText();
						fragColor = vec4(textColor.rgb * getBumpMapEffect(), textColor.a);
					}  else if (style == 3) {
						vec4 textColor = getTextWithOutline();
						fragColor = vec4(textColor.rgb * getBumpMapEffect(), textColor.a);
					}else {
						fragColor = getText();
					}
				}
				)";
	}; // Shader
}; // glat