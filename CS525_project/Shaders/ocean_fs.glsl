  
#version 430
out vec4 fragcolor;    

layout (binding = 1, rgba32f) readonly  uniform image2D normal_map;

in vec2 tex_coord;

uniform vec4 color;

void main(void)
{		
	vec3 n = imageLoad(normal_map, ivec2(tex_coord)).xyz;
	fragcolor = vec4(n, 1.0);
}

