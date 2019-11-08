#version 430

layout(location=0) in vec3 inputPosition;
layout(location=1) in vec2 inputTexture;
layout(location=2) in vec3 inputNormals;

out vec3 vertPos;
out vec2 texVS;

void main()
{
	gl_Position = vec4(vertPos, 1);
    vertPos = inputPosition;
	texVS = inputTexture;
}