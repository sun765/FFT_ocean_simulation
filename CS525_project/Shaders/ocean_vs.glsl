  
#version 430
out vec4 fragcolor;    
layout(location = 0) in vec3 pos_attrib; 

layout(rgba32f, binding = 0) uniform readonly image2D displacement_texture;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 tex_coord;

const float disp_scale = 5.0;

void main(void)
{		
	tex_coord = vec2(pos_attrib.x,  pos_attrib.z);
	vec3 disp = disp_scale * imageLoad(displacement_texture, ivec2(tex_coord)).rgb;
	//vec3 local_pos = vec3(pos_attrib.x, h, pos_attrib.z);

	vec3 local_pos = pos_attrib + disp;
	gl_Position = P*V*M*vec4(local_pos, 1.0);
}

