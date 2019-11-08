#version 430
uniform sampler2D Sampler;

in vec2 texVS;
in vec3 vertPos;
out vec4 fragColor;

void main()
{
	fragColor = texture(Sampler, texVS);
}