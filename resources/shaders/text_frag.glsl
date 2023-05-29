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
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = screenPxRange()*(sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    //fragColor = mix(bgColor, fgColor, opacity);
    vec3 textColor = vec3(1.0);
    fragColor = vec4(textColor, opacity);
}
