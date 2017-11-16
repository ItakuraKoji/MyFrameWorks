#include"ShaderClass.h"

ShaderClass::ShaderClass() {

}
ShaderClass::~ShaderClass() {
	glDetachShader(shaderProgram, fragmentShader);
	glDetachShader(shaderProgram, vertexShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(shaderProgram);
}


bool ShaderClass::Initialize(const char* vsFilename, const char* fsFilename) {
	GLint status;
	//一時的な文字列の格納先
	const char *vsResource, *fsResource;

	//頂点シェーダーとフラグメントシェーダー(ピクセルシェーダー)を読み込み
	vsResource = LoadTxtResource(vsFilename);
	if (!vsResource) {
		return false;
	}
	fsResource = LoadTxtResource(fsFilename);
	if (!fsResource) {
		return false;
	}

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vsResource, NULL);
	glShaderSource(fragmentShader, 1, &fsResource, NULL);

	//シェーダーリソースを作成したので文字列バッファ二つはもう用済み
	delete[] vsResource;
	vsResource = 0;
	delete[] fsResource;
	fsResource = 0;

	//シェーダーをコンパイル
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//エラーチェック
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		std::cout << "VertexShader Compile is Failed " << vsFilename << std::endl;
		ShowShaderErrors(vertexShader);
		system("pause");
		return false;
	}
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		std::cout << "FragmetShader Compile is Failed " << fsFilename << std::endl;
		ShowShaderErrors(fragmentShader);
		system("pause");

		return false;
	}

	//シェーダーをくっつけるプログラムを生成
	shaderProgram = glCreateProgram();
	//シェーダーをアタッチ
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//シェーダー内の変数のロケーションを指定
	glBindAttribLocation(shaderProgram, 0, "inputPos");
	glBindAttribLocation(shaderProgram, 1, "inputTexCoord");
	glBindAttribLocation(shaderProgram, 2, "inputNormal");
	glBindAttribLocation(shaderProgram, 3, "inputWeight");
	glBindAttribLocation(shaderProgram, 4, "inputIndex");


	//プログラムをリンク
	glLinkProgram(shaderProgram);

	//エラーチェック
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE) {
		std::cout << "Program Link Error" << std::endl;
		ShowLinkErrors(shaderProgram);
		system("pause");
		return false;
	}
	glUseProgram(shaderProgram);
	return true;
}

//シェーダーをセット
void ShaderClass::SetShader() {
	glUseProgram(shaderProgram);
}
bool ShaderClass::SetSkinningFunc(int in) {
	GLuint index;
	if (in) {
		index = glGetSubroutineIndex(shaderProgram, GL_VERTEX_SHADER, "NotSkinning");
	}
	else {
		index = glGetSubroutineIndex(shaderProgram, GL_VERTEX_SHADER, "CalcBoneMat");
	}
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &index);
	return true;
}

//シェーダーのUniform変数に値を渡す
bool ShaderClass::SetShaderParameter(float *world, float *view, float *projection) {
	GLint location;
	//ビュー
	location = glGetUniformLocation(shaderProgram, "view");
	if (location == -1) {
		return false;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, view);

	//投影
	location = glGetUniformLocation(shaderProgram, "projection");
	if (location == -1) {
		return false;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, projection);

	//ワールド
	location = glGetUniformLocation(shaderProgram, "world");
	if (location == -1) {
		return false;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, world);

	return true;
}

bool ShaderClass::SetShaderTexture(GLuint textureLayer, GLuint textureNumber) {
	GLint location;
	location = glGetUniformLocation(shaderProgram, "sampler");
	if (location == -1) {
		return false;
	}
	glActiveTexture(GL_TEXTURE0 + textureLayer);

	glUniform1i(location, textureLayer);
	glBindTexture(GL_TEXTURE_2D, textureNumber);
	return true;
}
bool ShaderClass::SetShaderTexture2(GLuint textureLayer, GLuint textureNumber) {
	GLint location;
	location = glGetUniformLocation(shaderProgram, "boneTex");
	if (location == -1) {
		return false;
	}
	glActiveTexture(GL_TEXTURE0 + textureLayer);

	glUniform1i(location, textureLayer);
	glBindTexture(GL_TEXTURE_2D, textureNumber);
	return true;
}

bool ShaderClass::SetShaderNumBone(int num) {
	GLint location = glGetUniformLocation(shaderProgram, "numBone");
	if (location == -1) {
		return false;
	}
	glUniform1i(location, num);
	return true;
}

bool ShaderClass::SetShaderBoneMatrix(float* boneMat) {
	GLint location = glGetUniformLocation(shaderProgram, "boneMat");
	if (location == -1) {
		return false;
	}
	glUniformMatrix4fv(location, 70, GL_FALSE, boneMat);
	return true;
}

bool ShaderClass::SetLightParameter(const Vector3f& direction, const Vector4f& color, const float power) {
	GLint location;
	location = glGetUniformLocation(shaderProgram, "lightDir");

	if (location == -1) {
		return false;
	}
	glUniform3f(location, direction.x(), direction.y(), direction.z());

	location = glGetUniformLocation(shaderProgram, "lightColor");
	if (location == -1) {
		return false;
	}
	glUniform4f(location, color.x(), color.y(), color.z(), color.w());

	location = glGetUniformLocation(shaderProgram, "lightPower");
	if (location == -1) {
		return false;
	}
	glUniform1f(location, power);
	return true;
}
bool ShaderClass::SetAmbientParameter(const Vector4f& color, const float power) {
	GLint location;
	location = glGetUniformLocation(shaderProgram, "ambientColor");
	if (location == -1) {
		return false;
	}
	glUniform4f(location, color.x(), color.y(), color.z(), color.w());

	location = glGetUniformLocation(shaderProgram, "ambientPower");
	if (location == -1) {
		return false;
	}
	glUniform1f(location, power);
	return true;
}

//シェーダーのテキストファイルを読み込んで文字列を格納したバッファの先頭ポインタを返す
char* ShaderClass::LoadTxtResource(const char* filename) {
	std::ifstream fin;
	char* returnSource;
	char input;
	int fileSize;

	fin.open(filename);

	if (fin.fail()) {
		return 0;
	}


	fileSize = 0;

	fin.get();
	while (!fin.eof()) {
		fin.get(input);
		++fileSize;
	}
	fin.close();

	returnSource = new char[fileSize + 1];
	if (!returnSource) {
		return 0;
	}

	fin.open(filename);
	fin.read(returnSource, fileSize);
	fin.close();
	

	returnSource[fileSize] = '\0';

	return returnSource;
}

//エラーメッセージをコンソールへ出力
void ShaderClass::ShowShaderErrors(GLuint shaderID) {
	GLint length;
	char* errorLog;

	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
	errorLog = new char[length + 1];

	glGetShaderInfoLog(shaderID, length, NULL, errorLog);

	for (int i = 0; i < length; ++i) {
		std::cout << errorLog[i];
	}
	std::cout << std::endl;
}
//エラーメッセージをコンソールへ出力
void ShaderClass::ShowLinkErrors(GLuint shaderID) {
	GLint length;
	char* errorLog;

	glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &length);
	errorLog = new char[length + 1];

	glGetProgramInfoLog(shaderID, length, NULL, errorLog);

	for (int i = 0; i < length; ++i) {
		std::cout << errorLog[i];
	}
	std::cout << std::endl;
}