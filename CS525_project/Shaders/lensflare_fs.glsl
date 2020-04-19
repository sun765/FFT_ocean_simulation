
#version 430
out vec4 fragcolor;    
layout(location = 61) uniform sampler2D flare;
layout(location = 62) uniform float strength;
layout(location = 63) uniform float fade;
in vec2 texCoord;


void main(void)
{		
	vec4 color = texture2D(flare,texCoord);
	fragcolor = color  ;
	fragcolor.a *=(strength*fade);
	

}

