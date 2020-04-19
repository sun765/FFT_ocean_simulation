#version 430   
layout(location = 0) in vec3 pos_attrib; 
layout(location = 1) in vec2 tex_coord; 

// try not using this first 
layout(location = 59) uniform vec2 sun_screen_pos; //(-1 to 1)
layout(location = 60) uniform vec2 sunScale;
layout(location = 58) uniform vec2 window_wh;

layout(location = 12) uniform mat4 PVM;



out vec2 wh;
out vec3 debug;
out vec2 texCoord;

vec3 posWindow(vec3 p);

void main(void)
{

   vec3 sunClipPos = vec3(sun_screen_pos,1.0);
   vec3 vertClipPos =posWindow(sunClipPos);
   gl_Position = vec4(vertClipPos.xy,0.9999, 1.0);
   texCoord = tex_coord;
   //gl_Position = vec4(pos_attrib, 1.0);

}


vec3 posWindow(vec3 p)
{
	float x = p.x + pos_attrib.x*(sunScale.x/window_wh.x);
	float y = p.y + pos_attrib.y*(sunScale.y/window_wh.y);
	return vec3(x,y,0.0);
}



















