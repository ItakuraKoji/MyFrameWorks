#include"ShaderList.h"

namespace K_Graphics {
	////////
	//public
	////
	ShaderList::ShaderList() {
		Initialize();
	}
	ShaderList::~ShaderList() {
		Initialize();
	}

	void ShaderList::Initialize() {
		for (auto i : this->shaderList) {
			delete i.second;
		}
		for (auto i : this->vertexShaders) {
			glDeleteShader(i.second);
		}
		for (auto i : this->pixelShaders) {
			glDeleteShader(i.second);
		}

		this->shaderList.clear();
		this->vertexShaders.clear();
		this->pixelShaders.clear();
		return;
	}

	//���_�V�F�[�_�[�쐬
	void ShaderList::LoadVertexShader(const std::string& fileName) {
		char* shaderResource = LoadTxtResource(fileName);

		if (shaderResource == nullptr) {
			throw("VertexShader Load Failed : " + fileName);
		}

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &shaderResource, NULL);

		delete[] shaderResource;

		//�V�F�[�_�[���R���p�C��
		glCompileShader(vs);

		//�G���[�`�F�b�N
		GLint status;
		glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			ShowShaderErrors(vs);
			throw("VertexShader Compile Failed : " + fileName);
		}
		this->vertexShaders[fileName] = vs;
	}
	//�s�N�Z���V�F�[�_�[�쐬
	void ShaderList::LoadFragmentShader(const std::string& fileName) {
		char* shaderResource = LoadTxtResource(fileName);

		if (shaderResource == nullptr) {
			throw("FragmentShader Load Failed : " + fileName);
		}

		GLuint ps = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(ps, 1, &shaderResource, NULL);

		delete[] shaderResource;

		//�V�F�[�_�[���R���p�C��
		glCompileShader(ps);

		//�G���[�`�F�b�N
		GLint status;
		glGetShaderiv(ps, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			ShowShaderErrors(ps);
			throw("FragmentShader Compile Failed : " + fileName);
		}
		this->pixelShaders[fileName] = ps;
	}

	//���X�g����V�F�[�_�[���擾�A������UseProgram()�A�L�[�����݂��Ȃ��Ƃ���NULL��Ԃ�
	ShaderClass* ShaderList::UseShader(const std::string& name) {
		if (this->shaderList.find(name) == shaderList.end()) {
			return 0;
		}
		this->shaderList[name]->UseShader();
		return this->shaderList[name];
	}
	//���X�g����V�F�[�_�[���擾�A�L�[�����݂��Ȃ��Ƃ���NULL��Ԃ�
	ShaderClass* ShaderList::GetShader(const std::string& name) {
		if (this->shaderList.find(name) == shaderList.end()) {
			return 0;
		}
		return this->shaderList[name];
	}

	//�쐬�ς݂̃V�F�[�_�[�̑g�ݍ��킹�ŃV�F�[�_�[�v���O�����쐬�A���X�g�֓o�^
	void ShaderList::CreateShaderProgram(const std::string& name, const std::string& vertex, const std::string& fragment) {
		if (this->vertexShaders.find(vertex) == this->vertexShaders.end()) {
			throw("VertexShader Not Found : " + name);
		}
		if (this->pixelShaders.find(fragment) == this->pixelShaders.end()) {
			throw("FragmentShader Shader Not Found : " + name);
		}

		ShaderClass* newShader = new ShaderClass(this->vertexShaders[vertex], this->pixelShaders[fragment]);
		if (newShader == nullptr) {
			throw("Shader Create is Failed : " + name);
		}
		this->shaderList[name] = newShader;
	}

	////////
	//private
	////


	//�V�F�[�_�[�̃e�L�X�g�t�@�C����ǂݍ���ŕ�������i�[�����o�b�t�@�̐擪�|�C���^��Ԃ�
	char* ShaderList::LoadTxtResource(const std::string& fileName) {
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
		if (returnSource == nullptr) {
			return 0;
		}

		fin.open(fileName);
		fin.read(returnSource, fileSize);
		fin.close();


		returnSource[fileSize] = '\0';

		return returnSource;
	}

	//�G���[���b�Z�[�W���R���\�[���֏o��
	void ShaderList::ShowShaderErrors(GLuint shaderID) {
		GLint length;
		char* errorLog;

		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		errorLog = new char[length + 1];

		glGetShaderInfoLog(shaderID, length, NULL, errorLog);

		for (int i = 0; i < length; ++i) {
			std::cout << errorLog[i];
		}
		std::cout << std::endl;

		delete[] errorLog;
	}
}
