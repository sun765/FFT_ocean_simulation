#version 430   
layout(location = 0) in vec3 pos_attrib; 
layout(location = 1) in vec2 tex_coord; 

layout(location = 56) uniform vec2 wh;
layout(location = 57) uniform vec2 screen_pos; // in window size
layout(location = 58) uniform vec2 window_wh;



out vec2 texCoord;

vec3 posWindow();
vec3 toClipCoord(vec3 p);

void main(void)
{
   vec3 vertWindowPos = posWindow();
   vec3 vertClipPos =toClipCoord(vertWindowPos);
   gl_Position = vec4(vertClipPos.xy, 0.999999, 1.0);
   texCoord = tex_coord;
   
}




vec3 posWindow()
{
	float x =screen_pos.x+ pos_attrib.x*wh.x*0.5;
	float y =screen_pos.y+ pos_attrib.y*wh.y*0.5;
	return vec3(x,y,0.0);
}

vec3 toClipCoord(vec3 p)
{
	float x = (p.x/window_wh.x)*2.0 -1.0;
	float y = (p.y/window_wh.y)*2.0 -1.0;
	return vec3(x,y,0.0);
}


















