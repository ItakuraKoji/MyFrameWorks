#version 400

//input
layout(location = 0)in vec3 inputPos;
layout(location = 1)in vec2 inputTexCoord;
layout(location = 2)in vec3 inputNormal;
layout(location = 3)in vec4 inputWeight;
layout(location = 4)in vec4 inputIndex;

//output
out vec4 outLightDepth;

//uniform
uniform mat4 matrixWVP;
uniform mat4 worldMatrix;
uniform mat4 lightMatrixVP;
uniform sampler2D boneTex;
uniform int numBone;

//skined
vec4 finalPos;
vec3 finalNormal;

//projection texture mapping matrix


//subroutine
subroutine void Skinning(vec3 vertexPos, vec3 vertexNormal);
subroutine uniform Skinning skinFunc;

//get BoneData with texture
mat4 GetBoneData(int boneIndex)
{
	vec4 m;
	vec2 uv;

	uv.y = 0.5f;

	uv.x = (boneIndex * 4.0f + 0.5f) / (numBone * 4.0f);
	m = texture2D(boneTex, uv);
	vec4 m1 = m;

	uv.x = (boneIndex * 4.0f + 1.5f) / (numBone * 4.0f);
	m = texture2D(boneTex, uv);
	vec4 m2 = m;
	
	uv.x = (boneIndex * 4.0f + 2.5f) / (numBone * 4.0f);
	m = texture2D(boneTex, uv);
	vec4 m3 = m;
	
	uv.x = (boneIndex * 4.0f + 3.5f) / (numBone * 4.0f);
	m = texture2D(boneTex, uv);
	vec4 m4 = m;


	return mat4(m1, m2, m3, m4);
}

subroutine(Skinning)
void CalcBoneMat(vec3 vertexPos, vec3 vertexNormal)
{
	vec4 pos = vec4(vertexPos, 1.0f);
	mat4 mat = mat4(0);

	mat += GetBoneData(int(inputIndex.w)) * inputWeight.w;
	mat += GetBoneData(int(inputIndex.z)) * inputWeight.z;
	mat += GetBoneData(int(inputIndex.y)) * inputWeight.y;
	mat += GetBoneData(int(inputIndex.x)) * inputWeight.x;

	
	finalPos = mat * pos;
	finalNormal = mat3(mat) * vertexNormal;

}

subroutine(Skinning)
void NotSkinning(vec3 vertexPos, vec3 vertexNormal)
{
	finalPos = vec4(vertexPos, 1.0f);
	finalNormal = vertexNormal;
}

//VertexShader
void main(void)
{
	skinFunc(inputPos, inputNormal);
	gl_Position = matrixWVP * finalPos;

	outLightDepth = lightMatrixVP * worldMatrix * finalPos;
}