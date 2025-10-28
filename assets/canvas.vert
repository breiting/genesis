#version 410 core
layout (location = 0) in vec2 iPos;

uniform mat4 uMVP;
out vec2 vPos;

void main() {
    vPos = iPos;
    gl_Position = uMVP * vec4(iPos, 0.0, 1.0);
}
