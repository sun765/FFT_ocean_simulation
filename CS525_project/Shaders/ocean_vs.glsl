  
#version 430
out vec4 fragcolor;    
layout(location = 0) in vec3 vertex_local_pos; 

/*
layout(rgba32f, binding = 0) uniform readonly image2D displacement_texture;
layout (binding = 1, rgba32f) readonly  uniform image2D normal_map;
*/

layout(binding = 1) uniform sampler2D normal_map;
layout(binding = 2) uniform sampler2D displacement_map;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 eye_world_pos;
//uniform float patch_size;

float patch_size = 256;
out vec2 tex_coord;
out vec3 view_dir;
out vec3 test_color;
out vec3 normal;
out float J;

const float disp_scale = 5.0;

void main(void)
{		

	int x = int(vertex_local_pos.x ) % int(patch_size);
	int z = int(vertex_local_pos.z ) % int(patch_size);
	tex_coord = vec2(float(x),  float(z)) / patch_size   + vec2(0.5)/256;
	//vec3 disp = disp_scale * imageLoad(displacement_texture, ivec2(tex_coord)).rgb;
	vec3 disp   = disp_scale *  texture(displacement_map, tex_coord).xyz;

	vec3 vertex_world_pos = (M * vec4(vertex_local_pos, 1.0)).xyz;
	view_dir = eye_world_pos - vertex_world_pos;
	test_color = eye_world_pos;

	//vec4 normal_j = imageLoad(normal_map, ivec2(tex_coord));
	vec4 normal_j   = texture(normal_map, tex_coord);
	normal = normal_j.xzy;
	J = normal_j.w;

	gl_Position = P*V*M*vec4(vertex_local_pos + disp, 1.0);
	//gl_Position = P*V*M*vec4(vertex_local_pos, 1.0);
}

