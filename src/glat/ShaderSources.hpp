namespace glat {
	namespace ShaderSource {
		///// QUADSTRIP

		static const char*	vertQuadStripShaderSource = R"(
				#version 330
				uniform mat4 modelViewProjection;

				layout (location = 0) in vec3 position;
				layout (location = 1) in vec2 textureCoord;
				layout (location = 2) in vec2 textureCoord2;
				layout (location = 3) in float textureSwitch;
				out float v_uv;
				out vec2 tex1;
				out vec2 tex2;

				void main()
				{
					v_uv = textureSwitch;
					tex1 = textureCoord;
					tex2 = textureCoord2;
					gl_Position = modelViewProjection * vec4(position, 1.0);
				}
				)";

		static const char* fragQuadStripShaderSource = R"(
				#version 330
				uniform sampler2D source;

				layout (location = 0) out vec4 fragColor;

				in float v_uv;
				in vec2 tex1;
				in vec2 tex2;

				void main()
				{
					fragColor = step(0.9999, abs(v_uv)) * texture2D(source, tex1) + (( 1 - step(0.9999, abs(v_uv))) * texture2D(source, tex2));
				}
				)";


		//// QUAD

		static const char*	vertQuadShaderSource = R"(
				#version 330

				uniform mat4 modelViewProjection;

				layout (location = 0) in vec3 position;
				layout (location = 1) in vec2 textureCoord;
				out vec2 v_uv;

				void main()
				{
					v_uv = textureCoord;
					gl_Position = modelViewProjection * vec4(position, 1.0);
				}
				)";

		static const char* fragQuadShaderSource = R"(
				#version 330

				uniform sampler2D source;

				layout (location = 0) out vec4 fragColor;

				in vec2 v_uv;

				void main()
				{
					fragColor = texture2D(source, v_uv);
				}
				)";

		static const char* fragDFQuadShaderSource = R"(
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
					float d = texture2D(source, v_uv).x - 0.5;	

					if (d < 0.0) {
						return vec4(textColor.rgb, textColor.a * smoothstep(0.0, 0.03, abs(d)));
					} 
					else {
						discard;
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