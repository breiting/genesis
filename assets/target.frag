#version 330 core

in vec2 vUV;

uniform float uRadius;
uniform float uBlendWidth;
uniform vec4  uColor;

out vec4 FragColor;

void main() {
    float dist = length(vUV) * uRadius;

    if (dist > uRadius) {
        discard;
    }

    float alpha = 1.0;
    if (dist > uRadius - uBlendWidth) {
        alpha = smoothstep(uRadius, uRadius - uBlendWidth, dist);
    }

    FragColor = vec4(uColor.rgb, uColor.a * alpha);
}
