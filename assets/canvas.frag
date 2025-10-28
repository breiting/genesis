#version 410 core

in vec2 vPos;
out vec4 FragColor;

uniform vec2 uSize;
uniform float uRadius;
uniform vec3 uBgColor;
uniform vec3 uGlassColor;
uniform float uAlpha;
uniform float uFrost;

// --- Rounded rectangle SDF ---
float roundedBoxSDF(vec2 p, vec2 b, float r)
{
    vec2 q = abs(p) - b + vec2(r);
    return length(max(q, 0.0)) - r;
}

void main()
{
	vec2 p = vPos * uSize; // world - coordinates
    float dist = roundedBoxSDF(p, uSize * 0.5, uRadius);
    float inside = smoothstep(0.0, -0.2, dist);

    // minor noise for milk-glass effect
    float noise = fract(sin(dot(vPos, vec2(12.9898,78.233))) * 43758.5453);
    vec3 frost = mix(uGlassColor, uGlassColor * 0.9, noise * uFrost);

    // Backgroud with  milk-glass
    vec3 color = mix(uBgColor, frost, inside * uAlpha);
    FragColor = vec4(color, 1.0);
}

