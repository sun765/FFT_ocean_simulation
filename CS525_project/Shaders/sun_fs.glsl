  
#version 430
out vec4 fragcolor;    
in vec2 texCoord;

in vec3 debug;
layout(location = 83) uniform sampler2D sun_texture;



void main(void)
{		
	vec4 tex_color = texture2D(sun_texture,texCoord);
	fragcolor = tex_color;

}

