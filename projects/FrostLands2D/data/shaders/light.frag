#version 450 core

uniform sampler2D Textures[32];
uniform vec3 globalLight;

in vec4 Pos;
in vec4 Col;
in vec2 Tex;
in vec3 Nor;
in float TexID;
in mat4 modelOut;

out vec4 FragmentColor;

void main() {
    FragmentColor = texture(Textures[int(TexID)], Tex) * Col * vec4(globalLight.xyz, 1.0);
}