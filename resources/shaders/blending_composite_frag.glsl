#version 450 core
layout (location = 0) out vec4 frag;

// alpha buffer data
uniform sampler2D accum;
uniform sampler2D reveal;

// epsilon number
const float EPSILON = 0.00001f;

// calculate floating point numbers equality accurately
bool isApproximatelyEqual(float a, float b) {
    return abs(a - b) <= (abs(a) < abs(b) ? abs(b) : abs(a)) * EPSILON;
}

// get the max value between three values
float max3(vec3 v) {
    return max(max(v.x, v.y), v.z);
}

void main() {

    ivec2 coords = ivec2(gl_FragCoord.xy);
    float revealage = texelFetch(reveal, coords, 0).r;

    // save the blending and color texture fetch cost if there is not a transparent fragment
    if (isApproximatelyEqual(revealage, 1.0f))
        discard;

    // fragment color
    vec4 accumulation = texelFetch(accum, coords, 0);

    // suppress overflow
    if (isinf(max3(abs(accumulation.rgb))))
        accumulation.rgb = vec3(accumulation.a);

    // prevent floating point precision bug
    vec3 average_color = accumulation.rgb / max(accumulation.a, EPSILON);

    // blend pixels
    frag = vec4(average_color, 1.0f - revealage);
}