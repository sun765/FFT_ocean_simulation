#version 430   
layout(location = 0) in vec3 pos_attrib; 
layout(location = 1) in vec2 tex_coord; 

//uniform
layout(location = 12) uniform mat4 PVM;

out vec3 pos;

void main(void)
{

   gl_Position = PVM*vec4(pos_attrib, 1.0);
   
   pos = pos_attrib;
}