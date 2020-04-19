#version 430


layout(location = 0) out vec4 fragcolor;  

layout(location = 32) uniform vec3 eyeWPos;
layout(location = 34) uniform vec3 lightPos;
layout(location = 43) uniform int meshId;
layout(location = 44) uniform int currentId;
layout(location = 24) uniform int pass;
layout(location = 45) uniform int  shading_mode;

layout(location = 0) in vec3 normal; 
layout(location = 1) in vec3 fragPos; 
layout(location = 2) in vec3 flatNormal; 

vec4 phong_light(vec3 n, vec3 l,vec3 v,vec3 r, vec3 Ld,vec3 Ls, float shineness, float d);

void main(void)
{		

	// computed simple phong light in world space
	vec3 n;
	if(shading_mode ==0)
	{
		n = normalize(normal);
	}
	else
	{
		n = normalize(flatNormal);
	}
	
	vec3 l = normalize(lightPos - fragPos);
	// hard
	l = normalize(vec3(1.0,1.0,-1.0));
	vec3 v = normalize(eyeWPos - fragPos);
	vec3 r = normalize(reflect(-l,n));

	vec3 Ld = vec3(0.0,0.8,0.8);
	vec3 Ls = vec3(1.0);
	float shineness = 0.5;
	float d = length(lightPos - fragPos);

	vec3 diffuse = Ld*0.8 *max(dot(n,l),0.0);

	if(pass == 3)
	{
		if(meshId==0)
		fragcolor = vec4(0.0);
		else
		fragcolor = vec4(1.0,0.0,0.0,0.0);
	}

	else if( pass ==5 )
	{
	if(meshId == currentId )
	{
		fragcolor = vec4(0.5);
	}
	else
	{
		fragcolor = vec4(diffuse,1.0);
	}
	}
	//fragcolor = phong_light(n,l,v,r,Ld,Ls,shineness,d);

}


vec4 phong_light(vec3 n, vec3 l,vec3 v,vec3 r, vec3 Ld,vec3 Ls, float shineness, float d)
{
	float Ka = 0.1;
	vec3 La = vec3(1.0);
	float Kd = 1.0;
	float Ks = 0.1;
	//float Ks = 0.1;
	vec3 n2 = vec3 (0.0,1.0,0.0);

	vec3 Ia = Ka*La;
	vec3 Id = Kd*Ld*max(dot(n,l),0.0);
	///(pow(d,1));
	vec3 Is= Ks*Ls*pow(max(dot(r,v),0.0),shineness)/(0.005*(pow(d,1.0)));

    vec3 I = Ia+ Ld +Is;
	return vec4(Id, 1.0);

}



















