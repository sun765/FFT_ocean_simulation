#version 430


//layout (location = 0) out vec4 fragcolor;            


layout(location = 0) out vec4 fragcolor;  
layout(location = 26) uniform sampler2D reflection;
layout(location = 27) uniform sampler2D reflaction;
layout(location = 28) uniform sampler2D dudv;
layout(location = 29) uniform float wave_strength;
layout(location = 30) uniform float time;
layout(location = 31) uniform float speed;
layout(location = 32) uniform vec3 eyeWPos;
layout(location = 33) uniform sampler2D normal_map;


layout(location = 38) uniform vec3 waterColor;
layout(location = 39) uniform float waterColorMix;
layout(location = 45) uniform int  shading_mode;
layout(location = 24) uniform int pass;
layout(location = 46) uniform int test;
layout(location = 47) uniform sampler2D hollowDepth;
layout(location = 50) uniform float clipDistance;
layout(location = 51) uniform float softness;
layout(location = 52) uniform float minBlueness;
layout(location = 53) uniform float maxBlueness;
layout(location = 54) uniform float murkyDepth;
layout(location = 22) uniform float height;

//dir light
layout(location = 34) uniform vec3 lightPos;
layout(location = 64) uniform float Ka;
layout(location = 65) uniform vec3 La;
layout(location = 66) uniform float Kd;
layout(location = 67) uniform vec3 Ld;
layout(location = 35) uniform float Ks;
layout(location = 36) uniform vec3 Ls;
layout(location = 37) uniform float shiness;

//fog color
layout(location = 40) uniform float density;
layout(location = 41) uniform float gradient;
layout(location = 42) uniform vec3  fog_color;

layout(location = 0) in vec4 ClipCoord;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 fragWPos;
layout(location = 3) in vec3 flatNormal;
layout(location = 5) in vec3 fragEPos;
layout(location = 6) in vec3 depthEPos;



vec4 phong_light(vec3 n, vec3 l,vec3 v,vec3 r, vec3 Ld, float shineness, float dist);

vec4 applyMurkiness(vec4 refractColour, float waterDepth);


void main(void)
{		
	
	vec3 NdcCoord = (ClipCoord.xyz/ClipCoord.w)/2.0 +vec3(0.5);
	vec2 reflectCoord = vec2(NdcCoord.x,1.0-NdcCoord.y);
	vec2 refractCoord = vec2(NdcCoord.x,NdcCoord.y);
	float move_factor = 0.005*time * speed;

	// a better distortion 
	//get the depth of the hollow 
	float hDepth = texture2D(hollowDepth,refractCoord).r;
	float wDepth = fragWPos.y/height;
	float waterDepth = hDepth+ wDepth;

	//waterDepth = 1.0;	
	waterDepth = clamp(waterDepth,0.0,1.0);
	waterDepth = pow(waterDepth, softness);
	vec2 distortedTexCoords = texture2D(dudv, vec2(texCoord.x + move_factor, texCoord.y)).rg*0.1;
	distortedTexCoords = texCoord + vec2(distortedTexCoords.x, distortedTexCoords.y+move_factor);
	vec2 totalDistortion = (texture2D(dudv, distortedTexCoords).rg * 2.0 - 1.0) * wave_strength*waterDepth;

	reflectCoord  += totalDistortion;
	refractCoord  += totalDistortion;

	// clamp to avoid wired edge
	clamp(reflectCoord,0.001,0.999);
	clamp(refractCoord,0.001,0.999);
	vec4 reflectColor = texture2D(reflection,reflectCoord);
	vec4 refractColor = texture2D(reflaction,refractCoord);

	// depth effect
	refractColor = applyMurkiness(refractColor, 10.0*waterDepth);

	//calculate refract factor based on the view angle
	vec3 view = eyeWPos - fragWPos;
	view = normalize(view);
	float refract_factor = dot(view,vec3(0.0,1.0,0.0));
	refract_factor = pow(refract_factor,2.0);
	vec4 normal_color  = texture2D(normal_map, distortedTexCoords);

	// compute lighting , all in world space
	// is this normal in world space?
	vec3 normal;
	if(shading_mode ==1)
	{
		normal = normalize(flatNormal);
	}
	else if(shading_mode== 0)
	{
		normal = normalize(vec3(normal_color.x*2.0-1.0,normal_color.z,normal_color.y*2.0-1.0));
	}
	vec3 lightdir = normalize(lightPos - fragWPos);
	vec3 reflectdir = normalize(reflect(-lightdir,normal));


	vec3 diffuse  = vec3(mix(reflectColor,refractColor, refract_factor));
	     diffuse  = mix(diffuse , waterColor, waterColorMix);

	float light_dist = length(lightPos -fragWPos);
	fragcolor = phong_light(normal,lightdir,view,reflectdir,diffuse,shiness,light_dist);

	float fog_dist = length(vec3(eyeWPos-fragWPos));
    float visibility = exp(-pow(0.0005*fog_dist*density,gradient));
	fragcolor = vec4(mix(fog_color,vec3(fragcolor), visibility),1.0);

	//fragcolor = vec4(diffuse,1.0);
	//fragcolor = vec4(Ka);
	//fragcolor = refractColor;
 
	fragcolor.a = waterDepth;
	

		if(pass ==4 &&test ==1) 
	{ 
	//make this to a uniform later;
		fragcolor  = vec4(waterDepth);
		//fragcolor = vec4(minBlueness);

	}
	

}

vec4 phong_light(vec3 n, vec3 l,vec3 v,vec3 r,vec3 diffuse,  float shineness, float d)
{

	vec3 Id = Kd*diffuse*max(dot(n,l),0.0);
	///(pow(d,1));
	vec3 Is=  Ks*Ls*pow(max(dot(r,v),0.0),shineness)/(0.005*(pow(d,1.0)));
	vec3 Ia = Ka*diffuse;

    vec3 I =Ia +Id +Is;
	return vec4(I , 1.0);

}

vec4 applyMurkiness(vec4 refractColour, float waterDepth){
	float murkyFactor = clamp(waterDepth / murkyDepth, 0.0, 1.0);
	float murkiness = minBlueness + murkyFactor * (maxBlueness - minBlueness);
	return vec4(mix(vec3(refractColour), waterColor, murkiness),1.0);
}




















