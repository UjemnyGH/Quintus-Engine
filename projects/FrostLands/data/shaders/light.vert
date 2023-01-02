#version 450 core

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 light_color;
uniform vec3 light_direction;

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aCol;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aNor;
layout(location = 4) in float aTexID;

out vec4 Pos;
out vec4 Col;
out vec2 Tex;
out vec3 Nor;
out float TexID;
out mat4 modelOut;
out vec4 l_color;
out vec3 l_direction;

void main() {
    gl_Position = (projection * view * model) * aPos;

    Pos = aPos;
    Col = aCol;
    Tex = aTex;
    Nor = mat3(transpose(inverse(model))) * aNor;
    TexID = aTexID;
    modelOut = model;
    l_color = light_color;
    l_direction = light_direction;
}