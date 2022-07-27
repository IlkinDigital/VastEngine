#vertex
#version 430 core

layout (location = 0) in vec3 a_Pos;

out vec3 v_TextureCoords;

uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * vec4(a_Pos, 1.0);
    v_TextureCoords = a_Pos;
}

#fragment
#version 430 core

layout (location = 0) out vec4 color;

in vec3 v_TextureCoords;

uniform samplerCube skybox;

void main()
{
    color = texture(skybox, v_TextureCoords);
}