#version 450 core
out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D msdf;

float screenPxRange() {
    return 4.5;
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    vec3 msd = texture(msdf, texCoords).rgb;
    ivec2 sz = textureSize(msdf, 0).xy;
    float dx = dFdx(texCoords.x) * sz.x; 
    float dy = dFdy(texCoords.y) * sz.y;
    float toPixels = 8.0 * inversesqrt(dx * dx + dy * dy);
    float sigDist = median(msd.r, msd.g, msd.b);
    float w = fwidth(sigDist);
    float opacity = smoothstep(0.5 - w, 0.5 + w, sigDist);
    vec3 textColor = vec3(1.0);
    fragColor = vec4(textColor, opacity);
}
