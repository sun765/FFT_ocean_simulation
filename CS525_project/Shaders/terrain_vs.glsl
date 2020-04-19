#version 430   
layout(location = 0) in vec3 pos_attrib; 
layout(location = 1) in vec2 tex_coord; 
layout(location = 2) in vec3 normal_attrib; 

//uniform
layout(location = 0) uniform mat4 M;
layout(location = 4) uniform mat4 V;
layout(location = 8) uniform mat4 P;
layout(location = 21) uniform sampler2D texture6;
layout(location = 16) uniform sampler2D texture1;
layout(location = 17) uniform sampler2D texture2;
layout(location = 18) uniform sampler2D texture3;
layout(location = 19) uniform sampler2D texture4;
layout(location = 20) uniform sampler2D texture5;
layout(location = 55) uniform mat4 depth_V;

layout(location = 73) uniform vec3 peak_color; 
layout(location = 74) uniform vec3 high_color; 
layout(location = 76) uniform vec3 middle_color; 
layout(location = 78) uniform vec3 under_color; 
layout(location = 75) uniform float high_height; 
layout(location = 77) uniform float middle_height; 
layout(location = 79) uniform float terrain_height; 
layout(location = 80) uniform float peak_height; 
layout(location = 81) uniform float under_height; 




layout(location = 25) uniform vec4 plane;


//layout(location = 25) uniform vec4 reflection_plane ;
//layout(location = 26) uniform vec4 refraction_plane ;

//layout(location = 0) out vec4 instanced_color; 
layout(location = 1) out vec2 texCoord;	
layout(location = 2) out vec3 pos;	
//layout(location = 2) flat out vec4 color;	
layout(location = 3) out vec3 normal;
layout(location = 4) out vec3 fragEPos; 
layout(location = 5) out float fogDistance; //tangent space to eye space
layout(location = 6) out vec3 fragWPos; //tangent space to eye space
layout(location = 9) out vec3 depthEPos;
layout(location = 10) out flat vec3 diffuse_color;
layout(location = 11) out vec3 fragClipPos;

vec3 cal_terrain_color(vec3 world_pos);
float to1(float a);

void main(void)
{

	 float yoffset = 2.0*texture2D(texture6,tex_coord/25.0).r-1.0;
	 vec3 offset= vec3(0.0,0.0,0.0);
	 // there's something wrong with clipDistance
	 gl_ClipDistance[0] = dot(vec4(pos_attrib,1.0),plane);
	 gl_Position = V*M*vec4(pos_attrib, 1.0);
	 texCoord = tex_coord;
	 normal = normalize(normal_attrib);

    fragEPos =vec3( V*M*vec4(pos_attrib, 1.0));
	vec4 fragCPos =vec4( P*V*M*vec4(pos_attrib, 1.0));
	fragClipPos =fragCPos.xyz/fragCPos.w;
	depthEPos =vec3( depth_V*M*vec4(pos_attrib, 1.0));
    fragWPos =vec3( M*vec4(pos_attrib, 1.0));
   	float fogDistance = length(fragEPos);

	vec3 test =  cal_terrain_color(fragWPos);

	diffuse_color = test;


}

vec3 cal_terrain_color(vec3 world_pos)
{
	float index = world_pos.y/terrain_height;
	index =  to1(index);
	float a = 0.0; 
	float b = to1(middle_height);
	float c = to1(high_height);
	float d = 1.0;
	float e = to1(under_height);
	float f = to1(peak_height);

	vec3 color1 = mix(under_color , middle_color, (index-a)/(b-a));
	vec3 color2 = mix(middle_color , high_color, (index-b)/(c-b));
	vec3 color3 = mix(high_color , peak_color, (index-c)/(d-c));
	return  (1.0-step(e,index))*under_color +
			(1.0-step(b,index))*color1 + 
			step(e,index)*(1.0-step(b,index))*color1 +
			step(b,index)*(1.0-step(c,index))*color2 +
			step(c,index)*(1.0-step(f,index))*color3 +
			step(f,index)*peak_color ;
	// return vec3(index );
}
float to1(float a)
{
	return (a+1.0)/2.0;
}