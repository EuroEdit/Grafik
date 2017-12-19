#version 330 core

uniform vec3 Color;

uniform vec3 AmbientLightColor;
uniform vec3 LightPosition;
uniform vec3 LightColor;

uniform vec3 EyePosition;

uniform vec3  AmbientColor;
uniform vec3  DiffuseColor;
uniform vec3  SpecularColor;
uniform float Shininess;

in vec3 WorldVertex;
in vec3 WorldNormal;

out vec4 FragColor;

void main() {
     vec3 color = vec3(0.0f, 0.0f, 0.0f);

     vec3 ambientcolor  = AmbientLightColor * AmbientColor;
     color += ambientcolor;

     vec3 N = normalize(WorldNormal);
     vec3 L = normalize(LightPosition - WorldVertex);
     vec3 V = normalize(EyePosition - WorldVertex);
     vec3 R = normalize((2.0f * dot(N, L)) * N - L);

     vec3 diffusecolor = vec3(0.0f, 0.0f, 0.0f);
     float dotLN = dot(L, N);
     if (dotLN > 0.0f) {
         diffusecolor  = LightColor * DiffuseColor * dotLN;
     }
     color += diffusecolor;

     vec3 specularcolor = vec3(0.0f, 0.0f, 0.0f);
     float dotRV = dot(R, V);
     if (dotRV > 0.0f) {
	specularcolor = (LightColor * SpecularColor) * pow(dotRV, Shininess);
     }
     color += specularcolor;

     FragColor = vec4(color, 1.0f);
}
