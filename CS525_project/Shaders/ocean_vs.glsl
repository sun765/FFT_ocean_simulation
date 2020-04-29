  
#version 430
out vec4 fragcolor;    
layout(location = 0) in vec3 pos_attrib; 

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main(void)
{		
	gl_Position = P*V*M*vec4(pos_attrib, 1.0);
}

