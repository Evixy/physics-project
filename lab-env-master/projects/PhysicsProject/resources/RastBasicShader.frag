#version 430
precision mediump float;
uniform sampler2D Sampler;

in vec2 texFrag;
in vec4 colorFrag;

uniform sampler2D tex;

out vec4 colorOut;

void main()
{		
	colorOut = texture(Sampler, texFrag) + colorFrag;
}