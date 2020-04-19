#version 430

layout(location = 0) out vec4 fragcolor;  
layout(location = 1) out vec4 depthColor;         


layout(location = 16) uniform sampler2D texture1;
layout(location = 17) uniform sampler2D texture2;
layout(location = 18) uniform sampler2D texture3;
layout(location = 19) uniform sampler2D texture4;
layout(location = 20) uniform sampler2D texture5;
layout(location = 47) uniform sampler2D depthTexture;
layout(location = 24) uniform int pass;
layout(location = 32) uniform vec3 eyeWPos;

layout(location = 40) uniform float density;
layout(location = 41) uniform float gradient;
layout(location = 42) uniform vec3  fog_color;
layout(location = 45) uniform int  shading_mode;
layout(location = 50) uniform float clipDistance;
layout(location = 22) uniform float height;

//layout(location = 0) in vec4 instanced_color; 
layout(location = 1) in vec2 texCoord;	
//layout(location = 2) flat in vec4 color;	
layout(location = 3) in vec3 normal;
layout(location = 4) in vec3 fragEPos; //tangent space to eye space
layout(location = 5) in float fogDistance; //tangent space to eye space
layout(location = 6) in vec3 fragWPos; //tangent space to eye space
layout(location = 7) in vec3 flatNormal; //tangent space to eye spacein vec3 flatNormal;
layout(location = 9) in vec3 depthEPos;

layout(location = 25) uniform vec4 plane;

void main(void)
{		
    // reading different terrain textures //
	vec4 blend = texture2D(texture1,texCoord/25.0);
	vec4 dirt = texture2D(texture2,texCoord);
	vec4 flower = texture2D(texture3,texCoord);
	vec4 grass = texture2D(texture4,texCoord);
	vec4 path = texture2D(texture5,texCoord);

	vec4 color1 = blend.g*flower +blend.r*dirt+ blend.b*path +(1.0-length(vec3(blend.rgb)))*grass ;
	// just a simple light model, will replace to better light
	vec3 dir = normalize(vec3(-1.0,-1.0,0.0));

	vec3 n;


	if(shading_mode == 0) // normal
	{
		n= normal;
	}
	else if(shading_mode ==1)
	{
		n = flatNormal;
	}
	float light = max(0.0,dot (dir,n));

	float dist = length(vec3(eyeWPos-fragWPos));
    float visibility = exp(-pow(0.0005*dist*density,gradient));

	vec4 test = vec4(0.494f, 0.686f, 0.176f,1.0);
	if(shading_mode==0)
	{
		fragcolor = light*grass;
	}
	else
	{
		fragcolor = light*test;
	}

	//hardcord the near far distance

	fragcolor = vec4(mix(fog_color,vec3(fragcolor), visibility),1.0);
	fragcolor = vec4(0.5);

	float depth = abs(depthEPos.z/clipDistance);
	depth = (abs(fragWPos.y/height));
	depthColor = vec4(depth);

	//fragcolor = vec4(gl_FragCoord.z);


}




















