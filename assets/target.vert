#version 330 core

layout (location = 0) in vec2 dummy;

uniform mat4 uMVP;
uniform vec2 uPos;
uniform float uRadius;

out vec2 vUV;

void main() {
    vec2 corner = vec2(
        (gl_VertexID == 1 || gl_VertexID == 3) ? 1.0 : -1.0,
        (gl_VertexID >= 2) ? 1.0 : -1.0
    );

    vUV = corner;

    vec2 worldPos = uPos + corner * uRadius;
    gl_Position = uMVP * vec4(worldPos, 0.0, 1.0);
}
