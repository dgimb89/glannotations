namespace glat {
	namespace ShaderSource {
		static const char*	vertexShaderSource = R"(
				#version 330

				uniform mat4 modelView;
				uniform mat4 projection;

				layout (location = 0) in vec4 position;
				layout (location = 1) in vec2 textureCoord;
				out vec2 v_uv;

				void main()
				{
					//v_uv = vec2(position.x, position.y) * 0.5 + 0.5;
					v_uv = textureCoord;
					gl_Position = projection * modelView * position;
				}
				)";

		static const char* fragmentShaderSource = R"(
				#version 330

				uniform sampler2D source;
				uniform int style;
				uniform vec4 textColor;
				uniform vec3 outlineColor;
				uniform float outlineSize;
				uniform float bumpIntensity;

				layout (location = 0) out vec4 fragColor;

				in vec2 v_uv;

				vec4 getText() {
					float d = texture2D(source, v_uv).x - 0.48;	

					if (d < 0.0) {
						return textColor;
					} 
					else {
						return vec4(0.0, 0.0, 0.0, 0.0);
					}
				}

				vec4 getTextWithOutline() {
					float d = texture2D(source, v_uv).x - 0.48;	

					// Interpolations Faktor zwischen outline und Welt
					float d_outline = smoothstep(outlineSize, 0.00 , d);

					if (d < 0.0) {
						return textColor;
					}
					else if (d_outline > 0.0) {
						return vec4(outlineColor, d_outline);
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

					return dot(n, lightSource);
				}

				void main()
				{
					if (style == 1) {
						fragColor = getTextWithOutline();
					} else if (style == 2) {
						vec4 text = getText();
						fragColor = vec4(text.rgb * getBumpMapEffect(), text.a);
					} else if (style == 3) {
						vec4 text = getTextWithOutline();
						fragColor = vec4(text.rgb * getBumpMapEffect(), text.a);
					} else {
						fragColor = getText();
					}
				}
				)";
	}; // Shader
}; // glat