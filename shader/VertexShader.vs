#version 400

//input
layout(location = 0)in vec3 inputPos;
layout(location = 1)in vec2 inputTexCoord;
layout(location = 2)in vec3 inputNormal;
layout(location = 3)in vec4 inputWeight;
layout(location = 4)in vec4 inputIndex;

//output
out vec2 outTexCoord;
out vec3 outNormal;
out vec4 outPosition;
out vec3 outEyeVector;

//uniform
uniform mat4 matrixWVP;
uniform mat4 worldMatrix;
uniform sampler2D boneTex;
uniform int numBone;
uniform vec3 cameraPos;


//skined
vec4 finalPos;
vec3 finalNormal;

//subroutine
subroutine void Skinning(vec3 vertexPos, vec3 vertexNormal);
subroutine uniform Skinning skinFunc;

//get BoneData with texture
mat4 GetBoneData(int boneIndex)
{
	vec4 m;
	vec2 uv;

	uv.y = 0.5;

	uv.x = (boneIndex * 4.0 + 0.5) / (numBone * 4.0);
	m = texture2D(boneTex, uv);
	vec4 m1 = m;

	uv.x = (boneIndex * 4.0 + 1.5) / (numBone * 4.0);
	m = texture2D(boneTex, uv);
	vec4 m2 = m;
	
	uv.x = (boneIndex * 4.0 + 2.5) / (numBone * 4.0);
	m = texture2D(boneTex, uv);
	vec4 m3 = m;
	
	uv.x = (boneIndex * 4.0 + 3.5) / (numBone * 4.0);
	m = texture2D(boneTex, uv);
	vec4 m4 = m;

	return mat4(m1, m2, m3, m4);
}

subroutine(Skinning)
void CalcBoneMat(vec3 vertexPos, vec3 vertexNormal)
{
	vec4 pos = vec4(vertexPos, 1.0f);
	mat4 mat = mat4(0);

	mat += GetBoneData(int(inputIndex.x)) * inputWeight.x;
	mat += GetBoneData(int(inputIndex.y)) * inputWeight.y;
	mat += GetBoneData(int(inputIndex.z)) * inputWeight.z;
	mat += GetBoneData(int(inputIndex.w)) * inputWeight.w;

	finalPos = mat * pos;
	finalNormal = mat3(mat) * vertexNormal;
}

subroutine(Skinning)
void NotSkinning(vec3 vertexPos, vec3 vertexNormal)
{
	finalPos = vec4(vertexPos, 1.0);
	finalNormal = vertexNormal;
}

//VertexShader
void main(void)
{
	skinFunc(inputPos, inputNormal);
	gl_Position = matrixWVP * finalPos;

	finalNormal = mat3(worldMatrix) * finalNormal;

	outPosition = gl_Position;
	outNormal = finalNormal;
	outTexCoord = inputTexCoord;
	outEyeVector = cameraPos - outPosition.xyz;
}