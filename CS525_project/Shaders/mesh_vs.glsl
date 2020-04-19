#version 430   
layout(location = 0) in vec3 pos_attrib; 
layout(location = 1) in vec2 tex_coord; 
layout(location = 2) in vec3 normal_attrib; 

//uniform
layout(location = 0) uniform mat4 M;
layout(location = 4) uniform mat4 V;
layout(location = 8) uniform mat4 P;

layout(location = 25) uniform vec4 plane;
//layout(location = 45) uniform mat4 M_update;


// what space is the normal in?
layout(location = 0) out vec3 normal; 
//layout(location = 1) out vec3 fragWPos; 


void main(void)
{

// ok this way?
   gl_ClipDistance[0] = dot(vec4(pos_attrib,1.0),plane);
   gl_Position = M*vec4(pos_attrib, 1.0);
   normal = normal_attrib;
   //fragWPos = vec3(M*vec4(pos_attrib, 1.0));
}