#vertex
#version 430 core

layout (location = 0) in vec3 a_Pos;

uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Pos, 1.0);
}

#fragment
#version 430 core
			
layout (location = 0) out vec4 color;

uniform sampler2D u_Texture;
			
void main()
{
	color = texture(u_Texture, vec2(0, 0));
}