#version 430
uniform mat4 model_in;
uniform mat4 proj_in;
uniform mat4 view_in;

uniform vec4 color_in;

layout(location=0) in vec3 inputPosition;
layout(location=1) in vec3 inputNormals;
layout(location=2) in vec2 inputTextures;

out vec4 colorFrag;
out vec2 texFrag;

void main()
{
	gl_Position = proj_in * view_in * model_in * vec4(inputPosition, 1.0f);
	colorFrag = color_in;
	texFrag = inputTextures;
};