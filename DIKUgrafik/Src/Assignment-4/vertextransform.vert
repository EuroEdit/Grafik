#version 330 core

uniform mat4x4 CTM;

in vec3 Vertex;
in vec3 Normal;

out vec3 WorldVertex;
out vec3 WorldNormal;

void main() {
    WorldVertex = Vertex;
    WorldNormal = Normal;
    gl_Position = CTM * vec4(Vertex, 1.0f);
}