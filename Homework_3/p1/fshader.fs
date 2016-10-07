#version 410 core

// Based on multi_vertex_lights.fs provided by Dr. Radkowski

in vec3 pass_Color;
out vec4 color;

void main(void)
{
    color = vec4(pass_Color, 1.0);
}
