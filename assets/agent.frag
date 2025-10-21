#version 330 core

in vec2 vUV;
in float vRadius;
in vec4 vColor;

out vec4 FragColor;

void main() {
    float dist = length(vUV) * vRadius;

    float coreZone = vRadius * 0.9;
    float fadeStart = coreZone;
    float fadeEnd = vRadius;

    if (dist <= coreZone) {
        // Inner solid core
        FragColor = vec4(vColor.rgb, vColor.a);
        return;
    }

    if (dist <= fadeEnd) {
        // Smooth fade from core to edge
        float alpha = smoothstep(fadeEnd, fadeStart, dist);
        FragColor = vec4(vColor.rgb, alpha * vColor.a);
        return;
    }

    discard; // Outside radius
}
