#version 330

varying vec2 vUV;

void main() {
    gl_FragColor = vec4(fract(vUV*15.), 0., vUV.x);
}