#version 400

out vec4 color;

//input
layout(location = 0)in vec3 inputPos;
layout(location = 1)in vec4 inputColor;

//uniform
uniform mat4 matrixWVP;

void main(void){
	gl_Position = matrixWVP * vec4(inputPos, 1.0f);
	color = inputColor;
}