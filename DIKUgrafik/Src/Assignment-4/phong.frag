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

vec3 computedAmbientColor( vec3 lightcolor,  vec3 ambientcolor) {
	vec3 color = lightcolor * ambientcolor;
	return color;
}

vec3 computedDiffuseColor( vec3 lightcolor,  vec3 diffusecolor,  float dotLN) {
	vec3 color = lightcolor * diffusecolor * max(0.0f, dotLN);
	return color;
}

vec3 computedSpecularColor( vec3 lightcolor,  vec3 specularcolor,  float shininess,  float dotRV) {
	vec3 color = (lightcolor * specularcolor) * pow(max(0.0f,dotRV), Shininess);
	return color;
}


void main() {
    vec3 color = vec3(0.0f, 0.0f, 0.0f);

    color += computedAmbientColor(AmbientLightColor, AmbientColor);

    vec3 N = normalize(WorldNormal);
    vec3 L = normalize(LightPosition - WorldVertex);
    vec3 V = normalize(EyePosition - WorldVertex);
    vec3 R = normalize((2.0f * dot(N, L)) * N - L);

    float dotLN = dot(L, N);
    color += computedDiffuseColor(LightColor, DiffuseColor, dotLN);

    float dotRV = dot(R, V);
    color += computedSpecularColor(LightColor, SpecularColor, Shininess, dotRV);

    FragColor = vec4(color, 1.0f);
}










