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
    vec4 PosModel = modelOut * Pos;

    /*float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);*/

    vec3 norm = normalize(Nor);
    vec3 lightDir = normalize(globalLight - vec3(PosModel.xyz));

    /*float diff = max(dot(norm, lightDir - vec3(PosModel.xyz)), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);*/

    /*vec4 result = vec4(ambient + diffuse, 1.0) * Col;
    FragmentColor = texture(Textures[int(TexID)], Tex) * result;*/

    float d = dot(lightDir, norm);

    FragmentColor = vec4(d + 0.1, d + 0.1, d + 0.1, 1.0);
}