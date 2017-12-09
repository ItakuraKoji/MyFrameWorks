#version 400

//output
out vec2 outTexCoord;

//input
layout(location = 0)in vec3 inputPos;
layout(location = 1)in vec2 inputTexCoord;


//uniform
uniform mat4 matrixWVP;

void main(void){
	gl_Position = matrixWVP * vec4(inputPos, 1.0f);
	outTexCoord = inputTexCoord;
}