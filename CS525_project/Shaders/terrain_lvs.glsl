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
	depthEPos =vec3( depth_V*M*vec4(pos_attrib, 1.0));
    fragWPos =vec3( M*vec4(pos_attrib, 1.0));
   	float fogDistance = length(fragEPos);


}