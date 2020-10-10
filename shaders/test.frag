#version 330

varying vec2 vTexCoords;

void main() {
    gl_FragColor = vec4(fract(vTexCoords*15.), 0., vTexCoords.x);
}