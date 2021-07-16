#type vertex

#version 450 core

layout (location = 0) in vec3 a_Positions;
layout (location = 1) in vec4 a_Colors;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in float a_TexIndex;
layout (location = 4) in float a_Layer;
layout (location = 5) in int a_EntityID;

out vec2 o_TexCoords;
out vec4 o_VertexColor;
out float o_TexIndex;
out flat int o_EntityID;

uniform mat4 u_ViewProjection = mat4(1.0);
void main()
{
	o_TexCoords = a_TexCoords;
	o_VertexColor = a_Colors;
	o_TexIndex = a_TexIndex;
	o_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * vec4(a_Positions.xy, a_Layer, 1.0);
}

#type fragment

#version 450 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int PickingID;

in vec2 o_TexCoords;
in vec4 o_VertexColor;
in float o_TexIndex;
in flat int o_EntityID;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 texColor = o_VertexColor;
	switch(int(o_TexIndex))
	{
		case 0: texColor *= texture(u_Textures[0], o_TexCoords); break;
		case 1: texColor *= texture(u_Textures[1], o_TexCoords); break;
		case 2: texColor *= texture(u_Textures[2], o_TexCoords); break;
		case 3: texColor *= texture(u_Textures[3], o_TexCoords); break;
		case 4: texColor *= texture(u_Textures[4], o_TexCoords); break;
		case 5: texColor *= texture(u_Textures[5], o_TexCoords); break;
		case 6: texColor *= texture(u_Textures[6], o_TexCoords); break;
		case 7: texColor *= texture(u_Textures[7], o_TexCoords); break;
		case 8: texColor *= texture(u_Textures[8], o_TexCoords); break;
		case 9: texColor *= texture(u_Textures[9], o_TexCoords); break;
		case 10: texColor *= texture(u_Textures[10], o_TexCoords); break;
		case 11: texColor *= texture(u_Textures[11], o_TexCoords); break;
		case 12: texColor *= texture(u_Textures[12], o_TexCoords); break;
		case 13: texColor *= texture(u_Textures[13], o_TexCoords); break;
		case 14: texColor *= texture(u_Textures[14], o_TexCoords); break;
		case 15: texColor *= texture(u_Textures[15], o_TexCoords); break;
		case 16: texColor *= texture(u_Textures[16], o_TexCoords); break;
		case 17: texColor *= texture(u_Textures[17], o_TexCoords); break;
		case 18: texColor *= texture(u_Textures[18], o_TexCoords); break;
		case 19: texColor *= texture(u_Textures[19], o_TexCoords); break;
		case 20: texColor *= texture(u_Textures[20], o_TexCoords); break;
		case 21: texColor *= texture(u_Textures[21], o_TexCoords); break;
		case 22: texColor *= texture(u_Textures[22], o_TexCoords); break;
		case 23: texColor *= texture(u_Textures[23], o_TexCoords); break;
		case 24: texColor *= texture(u_Textures[24], o_TexCoords); break;
		case 25: texColor *= texture(u_Textures[25], o_TexCoords); break;
		case 26: texColor *= texture(u_Textures[26], o_TexCoords); break;
		case 27: texColor *= texture(u_Textures[27], o_TexCoords); break;
		case 28: texColor *= texture(u_Textures[28], o_TexCoords); break;
		case 29: texColor *= texture(u_Textures[29], o_TexCoords); break;
		case 30: texColor *= texture(u_Textures[30], o_TexCoords); break;
		case 31: texColor *= texture(u_Textures[31], o_TexCoords); break;
	}

	FragColor = texColor;
	PickingID = o_EntityID;
}
