#version 330 core

in vec2 vUV;

out vec4 FragColor;

uniform vec2 uSize;
uniform float uCornerRadius;
uniform float uShadowSize;
uniform vec4 uShadowColor;
uniform vec4 uColor;

float roundedRectSDF(vec2 p, vec2 b, float r) {
    vec2 q = abs(p) - b + vec2(r);
    return length(max(q, 0.0)) - r;
}

void main() {
    // normalize to -1..1
    vec2 p = vUV * 2.0 - 1.0;
    vec2 halfSize = vec2(1.0);
    float dist = roundedRectSDF(p, halfSize - vec2(uShadowSize), uCornerRadius / uSize.x);

    float shadow = smoothstep(0.0, uShadowSize * 1.5, dist);
    vec3 color = mix(uShadowColor.rgb, uColor.rgb, shadow);
    FragColor = vec4(color, 1.0);
}
