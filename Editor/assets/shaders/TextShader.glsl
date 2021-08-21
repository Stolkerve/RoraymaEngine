#type vertex

#version 450 core

layout (location = 0) in vec3 a_Positions;
layout (location = 1) in vec4 a_Colors;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in float a_TexIndex;
layout (location = 4) in float a_Layer;

out vec2 o_TexCoords;
out vec4 o_VertexColor;
out float o_TexIndex;

uniform mat4 u_ViewProjection = mat4(1.0);
void main()
{
	o_TexCoords = a_TexCoords;
	o_VertexColor = a_Colors;
	o_TexIndex = a_TexIndex;
	gl_Position = u_ViewProjection * vec4(a_Positions.xy, a_Layer, 1.0);
}

#type fragment

#version 450 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out int PickingID;

in vec2 o_TexCoords;
in vec4 o_VertexColor;
in float o_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 texColor = o_VertexColor;
	switch(int(o_TexIndex))
	{
		case 0: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[0], o_TexCoords).r); break;
		case 1: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[1], o_TexCoords).r); break;
		case 2: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[2], o_TexCoords).r); break;
		case 3: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[3], o_TexCoords).r); break;
		case 4: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[4], o_TexCoords).r); break;
		case 5: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[5], o_TexCoords).r); break;
		case 6: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[6], o_TexCoords).r); break;
		case 7: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[7], o_TexCoords).r); break;
		case 8: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[8], o_TexCoords).r); break;
		case 9: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[9], o_TexCoords).r); break;
		case 10: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[10], o_TexCoords).r); break;
		case 11: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[11], o_TexCoords).r); break;
		case 12: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[12], o_TexCoords).r); break;
		case 13: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[13], o_TexCoords).r); break;
		case 14: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[14], o_TexCoords).r); break;
		case 15: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[15], o_TexCoords).r); break;
		case 16: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[16], o_TexCoords).r); break;
		case 17: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[17], o_TexCoords).r); break;
		case 18: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[18], o_TexCoords).r); break;
		case 19: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[19], o_TexCoords).r); break;
		case 20: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[20], o_TexCoords).r); break;
		case 21: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[21], o_TexCoords).r); break;
		case 22: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[22], o_TexCoords).r); break;
		case 23: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[23], o_TexCoords).r); break;
		case 24: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[24], o_TexCoords).r); break;
		case 25: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[25], o_TexCoords).r); break;
		case 26: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[26], o_TexCoords).r); break;
		case 27: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[27], o_TexCoords).r); break;
		case 28: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[28], o_TexCoords).r); break;
		case 29: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[29], o_TexCoords).r); break;
		case 30: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[30], o_TexCoords).r); break;
		case 31: texColor *= vec4(1.0, 1.0, 1.0, texture(u_Textures[31], o_TexCoords).r); break;
	}

	FragColor = texColor;
	PickingID = -1;
}
