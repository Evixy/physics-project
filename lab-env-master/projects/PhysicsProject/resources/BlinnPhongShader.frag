#version 430
uniform sampler2D Sampler;
uniform vec3 lightPos_in;
uniform vec3 ambientColor_in;
uniform vec3 diffuseColor_in;
uniform vec3 specColor_in;
uniform float shininess_in;

in vec2 texVS;
in vec3 normalInterp;
in vec3 vertPos;
out vec4 fragColor;

const vec3 lightPos = lightPos_in;
const vec3 ambientColor = ambientColor_in;
const vec3 diffuseColor = diffuseColor_in;
const vec3 specColor = specColor_in;
const float shininess = shininess_in;
const float screenGamma = 2.2; 

void main()
{
	vec3 normal = normalize(normalInterp);
	vec3 lightDir = normalize(lightPos - vertPos);
	
	float lambertian = max(dot(lightDir,normal), 0.0);
	float specular = 0.0;
	
	if(lambertian > 0.0) 
	{
		vec3 viewDir = normalize(-vertPos);
		vec3 halfDir = normalize(lightDir + viewDir);
		float specAngle = max(dot(halfDir, normal), 0.0);
		specular = pow(specAngle, shininess);
	}
	
	vec3 colorLinear = ambientColor + lambertian * diffuseColor + specular * specColor;
  
	vec3 colorGammaCorrected = pow(colorLinear, vec3(1.0/screenGamma));
	
	fragColor = texture(Sampler, texVS) * vec4(colorGammaCorrected, 1.0);
}