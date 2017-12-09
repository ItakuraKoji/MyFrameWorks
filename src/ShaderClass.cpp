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
		throw "�V�F�[�_�[�ǂݍ��ݎ��s";
	}
}
ShaderClass::~ShaderClass() {
	Finalize();
}


bool ShaderClass::Initialize(const std::string& vsFileName, const std::string& fsFileName) {
	Finalize();

	//�ꎞ�I�ȕ�����̊i�[��
	const char *vsResource, *fsResource;

	//���_�V�F�[�_�[�ƃt���O�����g�V�F�[�_�[(�s�N�Z���V�F�[�_�[)��ǂݍ���
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

	//�V�F�[�_�[���\�[�X���쐬�����̂ŕ�����o�b�t�@��͂����p�ς�
	delete[] vsResource;
	vsResource = 0;
	delete[] fsResource;
	fsResource = 0;

	//�V�F�[�_�[���R���p�C��
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//�G���[�`�F�b�N
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

	//�V�F�[�_�[����������v���O�����𐶐�
	shaderProgram = glCreateProgram();
	//�V�F�[�_�[���A�^�b�`
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);


	//�v���O�����������N
	glLinkProgram(shaderProgram);

	//�G���[�`�F�b�N
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

//�J��
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

//world * view * projection �̍����s���n��
void ShaderClass::SetMatrix(const Matrix4f& mat) {
	this->SetValue("matrixWVP", mat);
}
//���[���h�ϊ��s�񂾂���n��
void ShaderClass::SetWorldMatrix(const Matrix4f& world) {
	this->SetValue("worldMatrix", world);
}
//�e�N�X�`�����\�[�X�̔ԍ���n��
void ShaderClass::SetTexture(const std::string& uniformName, GLuint textureLayer, GLuint textureNumber) {
	GLuint location;
	location = glGetUniformLocation(this->shaderProgram, uniformName.data());
	if (location != -1) {
		glActiveTexture(GL_TEXTURE0 + textureLayer);
		glUniform1i(location, textureLayer);
		glBindTexture(GL_TEXTURE_2D, textureNumber);
	}
}
//�f�B���N�V���i�����C�g
void ShaderClass::SetDirectionalLight(float power, const Vector4f& color, const Vector3f& direction) {
	SetValue("lightPower", power);
	SetValue("lightColor", color);
	SetValue("lightDir", direction);
}
//����
void ShaderClass::SetAmbientLight(float power, const Vector4f& color) {
	SetValue("ambientPower", power);
	SetValue("ambientColor", color);
}

//�ėp�󂯓n��
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

//���I�V�F�[�_�[�ݒ�
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

//�V�F�[�_�[�̃e�L�X�g�t�@�C����ǂݍ���ŕ�������i�[�����o�b�t�@�̐擪�|�C���^��Ԃ�
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

//�G���[���b�Z�[�W���R���\�[���֏o��
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
//�G���[���b�Z�[�W���R���\�[���֏o��
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