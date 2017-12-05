#version 130
//precision highp float;

in vec3 color;

void main(){
    //color = vec4(1.0, 0.5, 0.5, 1.0);
    gl_FragColor = vec4(color, 1.0);
}
