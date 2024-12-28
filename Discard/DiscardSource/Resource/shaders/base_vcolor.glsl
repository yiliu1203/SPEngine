#type vertex
#version 410 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
layout(location = 2) in vec2 a_texCoord;

uniform mat4 u_viewprojection;
uniform mat4 u_model;
uniform vec3 u_emission;

out vec3 v_color;
out vec3 v_emission;

void main()
{
    gl_Position = u_viewprojection * u_model * vec4(a_position, 1.0);
    //gl_Position =  vec4(a_position, 1.0);
    v_color = a_color;
    v_emission = u_emission;
}


#type fragment
#version 410 core
layout(location=0) out vec4 FragColor;
in vec3 v_color;
in vec3 v_emission;

void main()
{
    FragColor = vec4(v_emission, 1.0);
}