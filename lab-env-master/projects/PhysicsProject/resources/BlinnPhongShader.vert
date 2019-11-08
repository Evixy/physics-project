#version 430
uniform mat4 model_in;
uniform mat4 proj_in;
uniform mat4 view_in;
const mat4 normal = transpose(inverse(model_in));
layout(location=0) in vec3 inputPosition;
layout(location=1) in vec2 inputTexture;
layout(location=2) in vec3 inputNormals;


out vec3 normalInterp;
out vec3 vertPos;
out vec2 texVS;

void main()
{
	gl_Position =  proj_in * view_in * model_in * vec4(inputPosition, 1.0f);
	vec4 vertPos4 = model_in * vec4(inputPosition, 1.0f);
    vertPos = vec3(vertPos4) / vertPos4.w;
    normalInterp = vec3(normal * vec4(inputNormals, 0.0f));
	texVS = inputTexture;
}