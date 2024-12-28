#type vertex
#version 410 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_texCoord;

uniform mat4 u_viewprojection;
uniform mat4 u_model;
uniform vec3 u_emission;
out vec2 v_texCoord;


void main()
{
    v_texCoord = a_texCoord;
    gl_Position = u_viewprojection * u_model * vec4(a_position, 1.0);
    
}


#type fragment
#version 410 core
layout(location=0) out vec4 FragColor;
uniform sampler2D u_texture;
in vec2 v_texCoord;


void main()
{
    FragColor = texture(u_texture, v_texCoord);
}