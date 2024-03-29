#version 460

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 position_eye, normal_eye;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;

void main(){
    position_eye = vec3(view * model * vec4(vertex_position, 1.0f));
    normal_eye   = vec3(view * model * vec4(vertex_normal, 0.0f));
    gl_Position  = proj * vec4(position_eye, 1.0f);
}