#version 130

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 in_pos;
in vec3 in_color;

out vec3 color;

void main(){
    vec4 pos = vec4(in_pos.xyz, 1.0);
    pos = projection * view * model * pos;

    gl_Position = pos;
    color = in_color;
}
