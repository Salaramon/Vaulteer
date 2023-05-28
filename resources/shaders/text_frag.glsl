#version 450 core
out vec4 fragColor;

in vec2 texCoords;

uniform sampler2DArray font;

void main() {
	fragColor = texture(font, vec3(texCoords, 0), 0);
	fragColor = vec4(1.0);
}