#version 330

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;

[VertProgram]

layout (location = 0) in vec2 in_position;
layout (location = 1) in vec4 in_color;

out vec4 color;

void main ()
{
    gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vec4(in_position,0,1);
    color = in_color;
}

[FragProgram]

in vec4 color;
out vec4 frag_color;

void main ()
{
    frag_color = color;
}
