namespace glat {
	namespace ShaderSource {
		///// QUADSTRIP

		static const char*	vertQuadStripShaderSource = R"(
				#version 330
				uniform mat4 modelViewProjection;

				layout (location = 0) in vec3 position;
				layout (location = 1) in vec2 texCoord;
				layout (location = 2) in vec2 texAdvance;
				layout (location = 3) in vec3 advanceH;
				layout (location = 4) in vec3 advanceW;
				
				out QuadData {
					vec2 texCoord;
					vec2 texAdvance;
					vec3 advanceH;
					vec3 advanceW;
					mat4 mvp;
				} quad;

				void main()
				{
					quad.texCoord = texCoord;
					quad.advanceH = advanceH;
					quad.advanceW = advanceW;
					quad.texAdvance = texAdvance;
					quad.mvp = modelViewProjection;
					gl_Position = vec4(position, 1.0);
				}
				)";

		static const char* geomQuadStripShaderSource = R"(
				#version 330

				layout(points) in;
				layout(triangle_strip, max_vertices = 4) out;

				in QuadData {
					vec2 texCoord;
					vec2 texAdvance;
					vec3 advanceH;
					vec3 advanceW;
					mat4 mvp;
				} quad[1];

				out VertexData {
					vec2 texCoord;
				} vertex;

				void main() {
					// ll
					//gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
					gl_Position = quad[0].mvp * gl_in[0].gl_Position;
					vertex.texCoord = quad[0].texCoord;
					EmitVertex();

					// ul
					gl_Position = quad[0].mvp * vec4(gl_in[0].gl_Position.xyz + quad[0].advanceH, 1.0);
					vertex.texCoord = quad[0].texCoord + vec2(0.0, quad[0].texAdvance.y);
					EmitVertex();

					// lr
					gl_Position = quad[0].mvp * vec4(gl_in[0].gl_Position.xyz + quad[0].advanceW, 1.0);
					vertex.texCoord = quad[0].texCoord.xy + vec2(quad[0].texAdvance.x, 0.0);
					EmitVertex();

					// ur
					gl_Position = quad[0].mvp * vec4(gl_in[0].gl_Position.xyz + quad[0].advanceH + quad[0].advanceW, 1.0);
					vertex.texCoord = quad[0].texCoord.xy + quad[0].texAdvance;
					EmitVertex();

					EndPrimitive();
				}
				)";

		static const char* fragQuadStripShaderSource = R"(
				#version 330
				uniform sampler2D source;
				uniform vec4 color;

				layout (location = 0) out vec4 fragColor;

				in VertexData {
					vec2 texCoord;
				} vertex;

				void main()
				{
					float distance = texture2D(source, vertex.texCoord);
					if(distance > 0.5) {
						discard;
					} else {
						fragColor = color;
					}
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
				uniform vec4 color;
				uniform vec3 outlineColor;
				uniform float outlineSize;
				uniform float bumpIntensity;

				layout (location = 0) out vec4 fragColor;

				in vec2 v_uv;

				vec4 getText() {
					float d = texture2D(source, v_uv).x - 0.5;	

					if (d < 0.0) {
						return vec4(color.rgb, color.a * smoothstep(0.0, 0.03, abs(d)));
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
						return color;
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