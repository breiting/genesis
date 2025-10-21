#version 330 core

layout (location = 1) in vec2 iPos;
layout (location = 2) in float iRadius;
layout (location = 3) in vec4 iColor;

out vec2 vUV;
out float vRadius;
out vec4 vColor;

uniform mat4 uMVP;

void main() {
    vec2 corner = vec2(
        (gl_VertexID == 1 || gl_VertexID == 3) ? 1.0 : -1.0,
        (gl_VertexID >= 2) ? 1.0 : -1.0
    );

    vUV = corner;
    vRadius = iRadius;
    vColor = iColor;

    vec2 worldPos = iPos + corner * iRadius;
    gl_Position = uMVP * vec4(worldPos, 0.0, 1.0);
}
