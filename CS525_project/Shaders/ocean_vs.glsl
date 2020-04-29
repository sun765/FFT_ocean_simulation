  
#version 430
out vec4 fragcolor;    
layout(location = 0) in vec3 pos_attrib; 

layout(rgba32f, binding = 0) uniform readonly image2D displacement_texture;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main(void)
{		
	ivec2 tex_coord = ivec2(pos_attrib.x,  pos_attrib.z);
	float h = 5.0*imageLoad(displacement_texture, tex_coord).r;
	vec3 local_pos = vec3(pos_attrib.x, h, pos_attrib.z);
	gl_Position = P*V*M*vec4(local_pos, 1.0);
}

