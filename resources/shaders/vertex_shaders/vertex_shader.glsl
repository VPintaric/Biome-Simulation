#version 130

uniform int left, width, top, height;
uniform mat3 transformMatrix;

in vec2 in_pos;
in vec3 in_color;

out vec3 color;

void main(){
    vec3 pos = vec3(in_pos.xy, 1.0);
    pos = transformMatrix * pos;
    pos = pos / pos.z;

    pos.x = ((pos.x - float(left)) / float(width));
    pos.y = ((pos.y - float(top)) / float(height));
    pos = pos * 2.0;
    pos = pos - 1.0;

    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
    color = in_color;
}
