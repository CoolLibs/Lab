#version 430

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aUV;

varying vec2 vUV;

void main() {
    gl_Position = vec4(aPosition, 0., 1.);
    vUV = aUV;
}
