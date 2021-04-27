#type vertex

#version 450 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_UV;

uniform mat4 u_ViewProjection = mat4(1.0);
uniform float u_CameraOrthoSize;

out vec2 o_TexCoords;
out float o_CameraOrthoSize;

void main()
{
	o_TexCoords = a_UV;
	o_CameraOrthoSize = u_CameraOrthoSize;
	gl_Position = u_ViewProjection * vec4(a_Pos, 1.0);
}

#type fragment

#version 450 core

layout (location = 0) out vec4 FragColor;

in vec2 o_TexCoords;
in float o_CameraOrthoSize;

float grid(vec2 st, float res)
{
	vec2 grid = fract(st * res);
	return (step(res, grid.x) * step(res, grid.y));
}

void main()
{
	vec2 coord = o_TexCoords.xy * o_CameraOrthoSize;
	float x = grid(coord, 100000.0);
	FragColor = vec4(vec3(1.0) * x, 1.0);
}
