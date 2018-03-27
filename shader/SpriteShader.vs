#version 400

//input
layout(location = 0)in vec3 inputPos;
layout(location = 1)in vec2 inputTexCoord;

//output
out vec2 outTexCoord;

//uniform
uniform mat4 matrixWVP;
uniform vec4 spriteUV;
uniform vec2 textureSize;


void main(void){
	gl_Position.x = inputPos.x;
	gl_Position.y = inputPos.y;
	gl_Position.z = inputPos.z;
	gl_Position.w = 1.0;
	gl_Position = matrixWVP * vec4(inputPos, 1.0);

	outTexCoord.x = (inputTexCoord.x * spriteUV.z + spriteUV.x) / textureSize.x;
	outTexCoord.y = 1.0 - ((1.0 - inputTexCoord.y) * spriteUV.w + spriteUV.y) / textureSize.y;
}