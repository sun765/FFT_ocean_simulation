#version 430   
layout(location = 0) in vec3 pos_attrib; 
layout(location = 1) in vec2 tex_coord; 

//uniform
layout(location = 0) uniform mat4 M;
layout(location = 4) uniform mat4 V;
layout(location = 8) uniform mat4 P;
layout(location = 55) uniform mat4 depth_V;


layout(location = 0) out vec4 ClipCoord;
layout(location = 1) out vec2 texCoord;
layout(location = 2) out vec3 fragWPos;
layout(location = 4) out vec3 pos;
layout(location = 5) out vec3 fragEPos;
layout(location = 6) out vec3 depthEPos;

/*out vec4 ClipCoord;
out vec2 texCoord;
out vec3 fragWPos;*/

void main(void)
{
	//object space
   ClipCoord = vec4(pos_attrib, 1.0);
   gl_Position = ClipCoord;
   texCoord = tex_coord;
   fragWPos = vec3(M*vec4(pos_attrib, 1.0));
   fragEPos = vec3(V*M*vec4(pos_attrib, 1.0));
   depthEPos = vec3(depth_V*M*vec4(pos_attrib, 1.0));
   pos = pos_attrib;

}