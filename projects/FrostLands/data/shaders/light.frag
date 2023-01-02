#version 450 core

uniform sampler2D Textures[32];

in vec4 Pos;
in vec4 Col;
in vec2 Tex;
in vec3 Nor;
in float TexID;
in mat4 modelOut;
out vec4 l_color;
out vec3 l_direction;

out vec4 FragColor;

void main() {
    float ambient_strenght = 0.1;
    vec4 ambient = ambient_strenght * l_color;

    vec4 frag_pos = modelOut * Pos;

    vec3 norm = normalize(Nor);
    vec3 light_dir = light_direction - vec3(frag_pos.xyz);

    float diff = max(dot(norm, light_dir), 0.0);
    vec4 diffuse = diff * l_color;

    vec4 color = (ambient + diffuse) * Col;

    if(int(TexID) == 0) {
        FragColor = color;
    }
    else {
        int tid = int(TexID);

        FragColor = texture(Textures[tid], Tex) * col;
    }
}