#version 430

layout(location = 0) out vec4 fragcolor;  
layout(location = 1) out vec4 depthColor;         

//for point light
layout(location = 68) uniform float ka ;
layout(location = 70) uniform float kd ;
layout(location = 69) uniform vec3 La ;
layout(location = 71) uniform vec3 Ld ;
layout(location = 72) uniform vec3 dir ;

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
layout(location = 10) in flat vec3 diffuse_color;
layout(location = 11) in vec3 fragClipPos;

layout(location = 25) uniform vec4 plane;
vec4 direction_light(vec3 col,vec3 n, vec3 l);
vec3 nomalize(vec3 p);

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

	vec3 n;


	if(shading_mode == 0) // normal
	{
		n= normalize(normal);
	}
	else if(shading_mode ==1)
	{
		n = normalize(flatNormal);
	}


	float dist = length(vec3(eyeWPos-fragWPos));
    float visibility = exp(-pow(0.0005*dist*density,gradient));

	vec3 test = vec3(0.494f, 0.686f, 0.176f);
	vec3 col ;
	if(shading_mode==0)
	{
		col = vec3(grass);
	}
	else
	{
		col = test;
	}
	vec3 d = normalize(dir);
	fragcolor = direction_light( diffuse_color, n,  d);


	//hardcord the near far distance

	fragcolor = vec4(mix(fog_color,vec3(fragcolor), visibility),1.0);
	if(pass ==3 )
	{
		vec3 p = nomalize(fragClipPos);
		fragcolor = vec4(p,1.0);
	}
	 //fragcolor = vec4(diffuse_color,1.0);

	float depth = abs(depthEPos.z/clipDistance);
	depth = (abs(fragWPos.y/height));
	depthColor = vec4(depth);

	//fragcolor = vec4(gl_FragCoord.z);


}

vec4 direction_light(vec3 col,vec3 n, vec3 l)
{
  vec3 ambient = col*ka;
  vec3 diffuse = col*kd*max(dot(n,l),0.0);
  return vec4(ambient+diffuse,1.0);
}

vec3 nomalize(vec3 p)
{
	return (p+vec3(1.0))/2.0;
}




















