  
#version 430
out vec4 fragcolor;    
layout(location = 0) in vec3 vertex_local_pos; 

layout(rgba32f, binding = 0) uniform readonly image2D displacement_texture;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 eye_world_pos;

out vec2 tex_coord;
out vec3 view_dir;
out vec3 test_color;

const float disp_scale = 5.0;

void main(void)
{		
	tex_coord = vec2(vertex_local_pos.x,  vertex_local_pos.z);
	vec3 disp = disp_scale * imageLoad(displacement_texture, ivec2(tex_coord)).rgb;

	vec3 vertex_world_pos = (M * vec4(vertex_local_pos, 1.0)).xyz;
	view_dir = eye_world_pos - vertex_world_pos;
	test_color = eye_world_pos;

	gl_Position = P*V*M*vec4(vertex_local_pos + disp, 1.0);
}

