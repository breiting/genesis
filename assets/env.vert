#version 330 core

layout (location = 0) in vec2 iPos;

uniform mat4 uMVP;

out vec2 vUV;

void main() {
    vUV = iPos * 0.5 + 0.5;
    gl_Position = uMVP * vec4(iPos, 0.0, 1.0);
}
