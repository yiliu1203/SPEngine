#shader vertex
#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;
out vec2 v_texCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    v_texCoords = texCoord;
}


#shader fragment
#version 410 core

layout(location=0) out vec4 FragColor;
in vec2 v_texCoords;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_texCoords);
    FragColor = texColor;
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    // FragColor = vec4(v_texCoords.x, v_texCoords.y, 0.0, 1.0);
}