#version 400

//input
layout(location = 0)in vec3 inputPos;
layout(location = 1)in vec2 inputTexCoord;
layout(location = 2)in mat4 instanceMat;
layout(location = 6)in vec4 instanceColor;

//output
out vec2 outTexCoord;
out vec4 outColor;

//uniform
uniform mat4 matrixWVP;

//VertexShader
void main(void)
{
	gl_Position = instanceMat * vec4(inputPos, 1.0f);
	outTexCoord = inputTexCoord;
	outColor = instanceColor;
}