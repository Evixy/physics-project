#version 430
precision mediump float;
uniform sampler2D Sampler;

in vec4 colorFrag;
in vec2 texFrag;

uniform sampler2D tex;

out vec4 colorOut;

void main()
{	
	colorOut = colorFrag;
	//colorOut = texture(Sampler, texFrag);
}