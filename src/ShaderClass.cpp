#include"ShaderClass.h"

////////
//public
////
ShaderClass::ShaderClass(const std::string& vsFilename, const std::string& fsFilename) {
	this->fragmentShader = 0;
	this->vertexShader = 0;
	this->shaderProgram = 0;
	if (!Initialize(vsFilename, fsFilename)) {
		Finalize();
		throw "シェーダー読み込み失敗";
	}
}
ShaderClass::~ShaderClass() {
	Finalize();
}


bool ShaderClass::Initialize(const std::string& vsFileName, const std::string& fsFileName) {
	Finalize();

	//一時的な文字列の格納先
	const char *vsResource, *fsResource;

	//頂点シェーダーとフラグメントシェーダー(ピクセルシェーダー)を読み込み
	vsResource = LoadTxtResource(vsFileName);
	if (!vsResource) {
		return false;
	}
	fsResource = LoadTxtResource(fsFileName);
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
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		std::cout << "VertexShader Compile is Failed " << vsFileName << std::endl;
		ShowShaderErrors(vertexShader);
		system("pause");
		return false;
	}
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		std::cout << "FragmetShader Compile is Failed " << fsFileName << std::endl;
		ShowShaderErrors(fragmentShader);
		system("pause");

		return false;
	}

	//シェーダーをくっつけるプログラムを生成
	shaderProgram = glCreateProgram();
	//シェーダーをアタッチ
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);


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

//開放
void ShaderClass::Finalize() {
	glDetachShader(shaderProgram, fragmentShader);
	glDetachShader(shaderProgram, vertexShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteProgram(shaderProgram);
}

//UseProgram
void ShaderClass::UseShader() {
	glUseProgram(this->shaderProgram);
}

//world * view * projection の合成行列を渡す
void ShaderClass::SetMatrix(const Matrix4f& mat) {
	this->SetValue("matrixWVP", mat);
}
//ワールド変換行列だけを渡す
void ShaderClass::SetWorldMatrix(const Matrix4f& world) {
	this->SetValue("worldMatrix", world);
}
//テクスチャリソースの番号を渡す
void ShaderClass::SetTexture(const std::string& uniformName, GLuint textureLayer, GLuint textureNumber) {
	GLuint location;
	location = glGetUniformLocation(this->shaderProgram, uniformName.data());
	if (location != -1) {
		glActiveTexture(GL_TEXTURE0 + textureLayer);
		glUniform1i(location, textureLayer);
		glBindTexture(GL_TEXTURE_2D, textureNumber);
	}
}
//ディレクショナルライト
void ShaderClass::SetDirectionalLight(float power, const Vector4f& color, const Vector3f& direction) {
	SetValue("lightPower", power);
	SetValue("lightColor", color);
	SetValue("lightDir", direction);
}
//環境光
void ShaderClass::SetAmbientLight(float power, const Vector4f& color) {
	SetValue("ambientPower", power);
	SetValue("ambientColor", color);
}

//汎用受け渡し
void ShaderClass::SetValue(const std::string& uniformName, int value) {
	GLuint location;
	location = glGetUniformLocation(this->shaderProgram, uniformName.data());
	if (location != -1) {
		glUniform1i(location, value);
	}
}
void ShaderClass::SetValue(const std::string& uniformName, float value) {
	GLuint location;
	location = glGetUniformLocation(this->shaderProgram, uniformName.data());
	if (location != -1) {
		glUniform1f(location, value);
	}
}
void ShaderClass::SetValue(const std::string& uniformName, const Matrix4f& value) {
	GLuint location;
	location = glGetUniformLocation(this->shaderProgram, uniformName.data());
	if (location != -1) {
		glUniformMatrix4fv(location, 1, GL_FALSE, value.data());
	}
}
void ShaderClass::SetValue(const std::string& uniformName, const Vector4f& value) {
	GLuint location;
	location = glGetUniformLocation(this->shaderProgram, uniformName.data());
	if (location != -1) {
		glUniform4fv(location, 1, value.data());
	}
}
void ShaderClass::SetValue(const std::string& uniformName, const Vector3f& value) {
	GLuint location;
	location = glGetUniformLocation(this->shaderProgram, uniformName.data());
	if (location != -1) {
		glUniform3fv(location, 1, value.data());
	}
}

//動的シェーダー設定
void ShaderClass::SetVertexShaderSubroutine(const std::string& subroutineFunctionName) {
	GLuint index = glGetSubroutineIndex(this->shaderProgram, GL_VERTEX_SHADER, subroutineFunctionName.data());
	if (index != GL_INVALID_INDEX) {
		glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &index);
	}
}
void ShaderClass::SetFragmentShaderSubroutine(const std::string& subroutineFunctionName) {
	GLuint index = glGetSubroutineIndex(this->shaderProgram, GL_FRAGMENT_SHADER, subroutineFunctionName.data());
	if (index != GL_INVALID_INDEX) {
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &index);
	}
}

////////
//private
////

//シェーダーのテキストファイルを読み込んで文字列を格納したバッファの先頭ポインタを返す
char* ShaderClass::LoadTxtResource(const std::string& fileName) {
	std::ifstream fin;
	char* returnSource;
	char input;
	int fileSize;

	fin.open(fileName);

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

	fin.open(fileName);
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