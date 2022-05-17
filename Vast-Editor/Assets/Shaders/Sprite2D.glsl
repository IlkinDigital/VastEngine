#vertex
#version 430 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TextureCoords;
layout (location = 3) in float a_TextureIndex;

out vec4 v_Color;
out float v_TextureIndex;
out vec2 v_TextureCoords;

uniform mat4 u_ViewProjection;

void main()
{
	v_Color = a_Color;
	v_TextureCoords = a_TextureCoords;
	v_TextureIndex = a_TextureIndex;
	gl_Position = u_ViewProjection * vec4(a_Pos, 1.0);
}

#fragment
#version 430 core
			
layout (location = 0) out vec4 color;
			
in vec4 v_Color;
in vec2 v_TextureCoords;
in float v_TextureIndex;

uniform sampler2D u_Textures[32];
			
void main()
{
	color = texture(u_Textures[int(v_TextureIndex)], v_TextureCoords) * v_Color;
}