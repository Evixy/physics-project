#version 430
uniform mat4 proj_in;
uniform mat4 view_in;

uniform vec4 color_in;

layout(location=0) in vec3 inputPosition;

out vec4 colorFrag;

void main()
{
	gl_Position = proj_in * view_in * vec4(inputPosition, 1.0f);
	colorFrag = color_in;
};