#version 430

varying vec2 vTexCoords;

// We use a smart trick explained here https://stackoverflow.com/a/59739538
// to draw fullscreen with a single triangle
// This is supposed to be more efficient than two triangles

void main() {
    vec2 vertices[3] = vec2[3](vec2(-1.,-1.), vec2(3.,-1.), vec2(-1., 3.));
    gl_Position = vec4(vertices[gl_VertexID], 0., 1.);
    vTexCoords = 0.5 * vertices[gl_VertexID] + vec2(0.5);
}