#version 430

layout(location = 23) uniform samplerCube cubemap;
//layout(location = 88) uniform samplerCube cubemap;
layout(location = 0) out vec4 fragcolor;  
layout(location = 1) out vec4 fragcolor1;        
in vec3 pos;


void main(void)
{		

	fragcolor = texture(cubemap,pos);
	fragcolor1 = fragcolor;
}




















