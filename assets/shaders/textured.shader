#version 330

uniform sampler2D u_texture0;

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;

[vertProgram]

layout (location = 0) in vec2 in_position;
layout (location = 1) in vec4 in_color;
layout (location = 2) in vec2 in_texcoord;

out vec4 color;
out vec2 texcoord;

void main ()
{
    gl_Position = u_projection_matrix * u_vieW_matrix * u_model_matrix * vec4(in_position,0,1);
    color = in_color;
    texcoord = in_texcoord;
}

[FragProgram]

in vec4 color;
in vec2 texcoord;

out vec4 frag_color;

void main ()
{
    frag_color = texture(u_texture0,texcoord) * vert_color;
}
